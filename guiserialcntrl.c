/** @file guiserialcntrl.c
 *  @brief Linux GUI to control arduino led status.
 *
 *  @author SKalkur
 *  @bug No know bugs.
 */

/* -- Includes -- */
#include <stdio.h>
#include <gtk/gtk.h>  /* \brief GTK+ files          */
#include <fcntl.h>  /* \brief File Control Definitions  */ 
#include <termios.h> /* \brief POSIX Terminal Control Definitions */
#include <unistd.h>  /* \brief UNIX Standard Definitions 	   */ 
#include <errno.h>   /* \brief ERROR Number Definitions           */
#include <time.h>


static int count;/*!< LED status monitor */
int fd;/*!<File Descriptor*/
FILE *fptr; /*!<File pointer used which is used to store the file */
char read_buffer[32];   /*!< Buffer to store the data received              */
int  bytes_read = 0;    /*!< Number of bytes read by the read() system call */
char port[] = "/dev/ttyACM0";/* Change /dev/ttyACM0 to the one corresponding to your system */

/** \fn void hello(GtkWidget *widget, gpointer data)
	*   \brief hello(GtkWidget *widget, gpointer data) function is the callback function when gtk window button is clicked. 
	*    \param *widget- GTK widget type
	* \param data- Additional info sent when button is clicked
	 **/
void hello(GtkWidget *widget, gpointer data)
{
	char* status;
	char write_buffer[25];/*!< Buffer containing characters to write into port	     */	
	int  bytes_written  = 0;  
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	count++;
	switch(count){
		case 1:
			status = "OFF";
			break;
		case 2:
			status = "ON";
			count=0;
			break;
		default:
			break;
	}
	g_print(">>%s %s\n",(char*)data,status); 
	write_buffer[0]=status[1];
	bytes_written = write(fd,write_buffer,1);/* use write() to send data to port                                            */
					     /* "fd"                   - file descriptor pointing to the opened serial port */
					     /*	"write_buffer"         - address of the buffer containing data	            */
					     /* 1-"sizeof(write_buffer)" - No of bytes to write                               */	
	printf("-- %s written to %s\n",status,port);
	fprintf(fptr,"LED Status=%s__%s",status,asctime(tm)); /* log the data in a text file */
}

/** \fn void destroy(GtkWidget* widget, gpointer data)
	*   \brief destroy(GtkWidget* widget, gpointer data) function is the callback function when gtk window close is clicked. 
	*    \param *widget- GTK widget type
	* \param data- Additional info sent when button is clicked
	 **/
void destroy(GtkWidget* widget, gpointer data)
{ 
     close(fd);
     fclose(fptr);
     gtk_main_quit(); 
} 


/** \fn int main( int  argc,  char *argv[] )  
	*   \brief main function is used to configure serial and gtk widgets
	*    \param argc command line arguement
	*    \param *argv[] command line arguement
	 **/
int main( int  argc,  char *argv[] )  
{
	printf("\n +----------------------------------+");
	printf("\n |        Serial Port Write         |");
	printf("\n +----------------------------------+");


	/*------------------------------- Opening the Serial Port -------------------------------*/
	
	fd = open(port,O_RDWR | O_NOCTTY | O_NDELAY);//"/dev/ttyACM0"	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */ 
		   						/* O_RDWR Read/Write access to serial port           */
								/* O_NOCTTY - No terminal will control the process   */
								/* O_NDELAY -Non Blocking Mode,Does not care about-  */
								/* -the status of DCD line,Open() returns immediatly */                                        
								
	if(fd == -1)						/* Error Checking */
    	   printf("\n  Error! in Opening %s  ",port);
	else
    	   printf("\n  %s Opened Successfully ",port);


	/*---------- Setting the Attributes of the serial port using termios structure --------- */
	
	struct termios SerialPortSettings;	/* Create the structure                          */

	tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */

	cfsetispeed(&SerialPortSettings,B9600); /* Set Read  Speed as 9600                       */
	cfsetospeed(&SerialPortSettings,B9600); /* Set Write Speed as 9600                       */

	SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
	SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
	SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */

	SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */ 
	
	
	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

	SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

	if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
	    printf("\n  ERROR ! in Setting attributes\n");
	else
            printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none\n");

	tcflush(fd,TCIFLUSH); /* Discards old data in the rx buffer            */

	/********************************************************************************/
	fptr = fopen("serData.txt","a");
	if (fptr == NULL){
        	printf("File does not exists \n");
	}
	/********************************************************************************/
	/*------------------------------- GTK+ window configuration -----------------------------*/
	GtkWidget *window;  
	GtkWidget *button;
	gtk_init (&argc, &argv);  
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);  
	gtk_container_set_border_width(GTK_CONTAINER(window), 50); 
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy), NULL); 
	button = gtk_button_new_with_label("LED ON/OFF");
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(hello), "LED"); 
	gtk_container_add(GTK_CONTAINER(window), button); 
	gtk_widget_show (window);  
	gtk_widget_show(button);
	gtk_main ();  
	return 0;  	
}
