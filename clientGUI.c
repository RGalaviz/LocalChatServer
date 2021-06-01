#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <pthread.h>
#include <ctype.h>
#include <stdbool.h>
#include "header.h"
// Make our widgets global

GtkWidget *window;
GtkWidget *FixedContainer;
GtkWidget *ConnectBtn;
GtkWidget *DisconnectBtn;
GtkWidget *SendBtn;
GtkWidget *LabelServerIp;
GtkWidget *LabelPortNumber;
GtkWidget *EntryServerIp;
GtkWidget *EntryPortNumber;
GtkWidget *ChatEntry;
GtkTreeStore *TreeStore;
GtkTreeView *TreeView;
GtkTreeViewColumn *cx1;
GtkCellRenderer *cr1;
GtkTreeSelection *selection;
GtkBuilder *builder;
GtkTreeIter iter;

bool connected = FALSE;
int sfd;
unsigned char message[1024];
unsigned char Sendmessage[1024];
int port;
char* ip;
struct sockaddr_in server_info;

/*To compile gcc `pkg-config --cflags gtk+-3.0` -o clientGui clientGUI.c `pkg-config --libs gtk+-3.0` -lpthread */

int main(int argc, char **argv) {

	gtk_init(&argc,&argv); // initializes Gtk

	//establish contact with xml code.

	builder = gtk_builder_new_from_file("./GladeProject/Glade.glade");
	//Creates widget window, associated with the "Window" property of our XML File
	window = GTK_WIDGET(gtk_builder_get_object(builder,"Window"));
	//Create our "X" signal, in order to destroy the window
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL); 

	//Connect GLADE established signals to C file Widgets
	gtk_builder_connect_signals(builder,NULL);
	FixedContainer =  GTK_WIDGET(gtk_builder_get_object(builder,"FixedContainer"));

	ConnectBtn = GTK_WIDGET(gtk_builder_get_object(builder,"ConnectBtn"));

	DisconnectBtn = GTK_WIDGET(gtk_builder_get_object(builder,"DisconnectBtn"));

	gtk_widget_set_sensitive (DisconnectBtn, FALSE);

	SendBtn = GTK_WIDGET(gtk_builder_get_object(builder,"SendBtn"));

	gtk_widget_set_sensitive (SendBtn, FALSE);

	LabelServerIp = GTK_WIDGET(gtk_builder_get_object(builder,"LabelServerIP"));

	LabelPortNumber = GTK_WIDGET(gtk_builder_get_object(builder,"LabelPort"));

	EntryServerIp = GTK_WIDGET(gtk_builder_get_object(builder,"ServerIP"));

	EntryPortNumber = GTK_WIDGET(gtk_builder_get_object(builder,"PortNumber"));

	ChatEntry = GTK_WIDGET(gtk_builder_get_object(builder,"ChatEntry"));

	TreeStore = GTK_TREE_STORE(gtk_builder_get_object(builder,"TreeStore"));

	TreeView = GTK_TREE_VIEW(gtk_builder_get_object(builder,"TreeView"));

	cx1 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder,"cx1"));

	cr1 = GTK_CELL_RENDERER(gtk_builder_get_object(builder,"cr1"));

	selection = GTK_TREE_SELECTION(gtk_builder_get_object(builder,"select"));

	gtk_tree_view_column_add_attribute(cx1,cr1,"text",0);

	

	gtk_tree_store_append(TreeStore,&iter,NULL);

	gtk_tree_store_set(TreeStore, &iter, 0, "SampleText", -1);

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(TreeView));
	//Show every widget created, even the window!
	gtk_widget_show_all(window);
	//Creates the main program loop.
	gtk_main();

	return EXIT_SUCCESS;
}

void *clientListen() {

	
	unsigned char answer[1024];
	//gtk_label_set_text(label,"text");
	// run the main loop to update the GUI and get it responsive:
	//while(gtk_events_pending()) gtk_main_iteration();

	// do some other computation...

	// huge computation in a loop:
	while(connected) {
		// do some computation...
		

		read(sfd, &answer, sizeof(answer));
		printf("-> %i\n", connected);
			
		gtk_tree_store_append(TreeStore,&iter,NULL);

		gtk_tree_store_set(TreeStore, &iter, 0, answer, -1);

		// update GUI and treat events from it:
		while(gtk_events_pending()) gtk_main_iteration();
	}

	
}

void on_SendBtn_clicked (GtkButton *b) {
	//Sendmessage = (char *)gtk_entry_get_text(GTK_ENTRY(ChatEntry));

	strcpy(Sendmessage,(char *)gtk_entry_get_text(GTK_ENTRY(ChatEntry)));

	printf("Message Send: %s\n", Sendmessage);
	if ((strlen (Sendmessage) > 0) && (Sendmessage[strlen(Sendmessage)-1] == '\n'))
    		Sendmessage[strlen(Sendmessage)-1] = '\0';
	write(sfd, &Sendmessage, sizeof(Sendmessage));

	gtk_entry_set_text(GTK_ENTRY(ChatEntry),"");
	
	gtk_tree_store_append(TreeStore,&iter,NULL);

	gtk_tree_store_set(TreeStore, &iter, 0, Sendmessage, -1);
}

void on_ConnectBtn_clicked (GtkButton *b) {
	port = atoi((char *)gtk_entry_get_text(GTK_ENTRY(EntryPortNumber)));
	ip = (char *)gtk_entry_get_text(GTK_ENTRY(EntryServerIp));
	
	printf("Port: %i \n",port);
	printf("Ip: %s \n", ip);

	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		printf("Error in socket creation\n");
		GtkWidget *dialog;
		dialog = gtk_message_dialog_new(GTK_WINDOW(window),
					GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_MESSAGE_INFO,
					GTK_BUTTONS_OK,
					"Error establishing socket connection");
		gtk_window_set_title(GTK_WINDOW(dialog), "Error");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
	}

	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(ip);
	server_info.sin_port = htons(port);
	if ( connect(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		printf("error in connection\n");
		GtkWidget *dialog;
		dialog = gtk_message_dialog_new(GTK_WINDOW(window),
					GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_MESSAGE_INFO,
					GTK_BUTTONS_OK,
					"Error connecting to Ip");
		gtk_window_set_title(GTK_WINDOW(dialog), "Error");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
	}else{
		GtkWidget *dialog;
		dialog = gtk_message_dialog_new(GTK_WINDOW(window),
					GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_MESSAGE_INFO,
					GTK_BUTTONS_OK,
					"Connection Completed");
		gtk_window_set_title(GTK_WINDOW(dialog), "Success!");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);

		
		gtk_widget_set_sensitive (ConnectBtn, FALSE);
		gtk_widget_set_sensitive (SendBtn, TRUE);
		gtk_widget_set_sensitive (DisconnectBtn, TRUE);
		pthread_t pthread_id;
		connected = TRUE;
		pthread_create(&pthread_id,NULL, clientListen,NULL);
	}
	
}
void on_DisconnectBtn_clicked (GtkButton *b) {
	unsigned char message[1024] = "Exit";
	connected = FALSE;
	write(sfd, &message, sizeof(message));
	close(sfd);
	printf("Sesión de chat finalizada\n");

	gtk_widget_set_sensitive (ConnectBtn, TRUE);
	gtk_widget_set_sensitive (SendBtn, FALSE);
	gtk_widget_set_sensitive (DisconnectBtn, FALSE);

}



