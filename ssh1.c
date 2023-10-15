#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h>
 #include <cstring>
 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <libssh/sftp.h>
#include <fcntl.h>
#define MAX_XFER_BUF_SIZE 16384
 
int sftp_read_async(ssh_session session, sftp_session sftp)
{
  int access_type;
  sftp_file file;
  char buffer[MAX_XFER_BUF_SIZE];
  int async_request;
  int nbytes;
  long counter;
  int rc;
 
  access_type = O_RDONLY;
  file = sftp_open(sftp, "elif",
                   access_type, 0);
  if (file == NULL) {
    fprintf(stderr, "Can't open file for reading: %s\n",
                     ssh_get_error(session));
    return SSH_ERROR;
  }
  sftp_file_set_nonblocking(file);
 
  async_request = sftp_async_read_begin(file, sizeof(buffer));
  counter = 0L;
  usleep(10000);
  if (async_request >= 0) {
    nbytes = sftp_async_read(file, buffer, sizeof(buffer),
                             async_request);
  } else {
      nbytes = -1;
  }
 
  while (nbytes > 0 || nbytes == SSH_AGAIN) {
    if (nbytes > 0) {
      write(1, buffer, nbytes);
      async_request = sftp_async_read_begin(file, sizeof(buffer));
    } else {
        counter++;
    }
    usleep(10000);
 
    if (async_request >= 0) {
      nbytes = sftp_async_read(file, buffer, sizeof(buffer),
                               async_request);
    } else {
        nbytes = -1;
    }
  }
 
  if (nbytes < 0) {
    fprintf(stderr, "Error while reading file: %s\n",
            ssh_get_error(session));
    sftp_close(file);
    return SSH_ERROR;
  }
 
  printf("The counter has reached value: %ld\n", counter);
 
  rc = sftp_close(file);
  if (rc != SSH_OK) {
    fprintf(stderr, "Can't close the read file: %s\n",
            ssh_get_error(session));
    return rc;
  }
 
  return SSH_OK;
}
int sftp_helloworld(ssh_session session, sftp_session sftp)
{
  int rc;
 
  rc = sftp_mkdir(sftp, "helloworld", S_IRWXU);
  if (rc != SSH_OK)
  {
    if (sftp_get_error(sftp) != SSH_FX_FILE_ALREADY_EXISTS)
    {
      fprintf(stderr, "Can't create directory: %s\n",
              ssh_get_error(session));
        return rc;
    }
  }
 
 // ...
 
  return SSH_OK;
}

int sftp_helloworld(ssh_session session)
{
  sftp_session sftp;
  int rc;
 
  sftp = sftp_new(session);
  if (sftp == NULL)
  {
    fprintf(stderr, "Error allocating SFTP session: %s\n",
            ssh_get_error(session));
    return SSH_ERROR;
  }
 
  rc = sftp_init(sftp);
  if (rc != SSH_OK)
  {
    fprintf(stderr, "Error initializing SFTP session: code %d.\n",
            sftp_get_error(sftp));
    sftp_free(sftp);
    return rc;
  }
// sftp_helloworld(session,sftp);
 sftp_read_async(session,sftp);
  //...
 
  sftp_free(sftp);
  return SSH_OK;
}
/*int scp_receive(ssh_session session, ssh_scp scp)
{
  int rc;
  int size, mode;
  char *filename, *buffer;
  rc = ssh_scp_pull_request(scp);
  if (rc != SSH_SCP_REQUEST_NEWFILE)
  {
    fprintf(stderr, "Error receiving information about file: %s\n",
            ssh_get_error(session));
    return SSH_ERROR;
  }
  size = ssh_scp_request_get_size(scp);
  filename = strdup(ssh_scp_request_get_filename(scp));
  mode = ssh_scp_request_get_permissions(scp);
  printf("Receiving file %s, size %d, permissions 0%o\n",
          filename, size, mode);
  free(filename);
  //buffer = malloc(size);
  buffer=(char*)malloc(size);
  if (buffer == NULL)
  {
    fprintf(stderr, "Memory allocation error\n");
    return SSH_ERROR;
  }
  ssh_scp_accept_request(scp);
  rc = ssh_scp_read(scp, buffer, size);
  if (rc == SSH_ERROR)
  {
    fprintf(stderr, "Error receiving file data: %s\n",
            ssh_get_error(session));
    free(buffer);
    return rc;
  }
  printf("Done\n%s",buffer);
  write(".", buffer, size);
  
  free(buffer);
  rc = ssh_scp_pull_request(scp);
  if (rc != SSH_SCP_REQUEST_EOF)
  {
    fprintf(stderr, "Unexpected request: %s\n",
            ssh_get_error(session));
    return SSH_ERROR;
  }
  return SSH_OK;
}
*/
int scp_receive(ssh_session session, ssh_scp scp)
{
  int rc;
  int size, mode;
  char *filename, *buffer;

  rc = ssh_scp_pull_request(scp);
  if (rc != SSH_SCP_REQUEST_NEWFILE)
  {
    fprintf(stderr, "Error receiving information about file: %s\n",
            ssh_get_error(session));
    return SSH_ERROR;
  }

  size = ssh_scp_request_get_size(scp);
  filename = strdup(ssh_scp_request_get_filename(scp));
  mode = ssh_scp_request_get_permissions(scp);
  printf("Receiving file %s, size %d, permisssions 0%o\n",
          filename, size, mode);
  free(filename);

  //buffer = malloc(size);
    buffer=(char*)malloc(size);
  if (buffer == NULL)
  {
    fprintf(stderr, "Memory allocation error\n");
    return SSH_ERROR;
  }

  ssh_scp_accept_request(scp);
  rc = ssh_scp_read(scp, buffer, size);
  if (rc == SSH_ERROR)
  {
    fprintf(stderr, "Error receiving file data: %s\n",
            ssh_get_error(session));
    free(buffer);
    return rc;
  }
  printf("Done!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

  int filedesc = open("elif", O_WRONLY | O_CREAT);

  if (filedesc < 0) {
    return -1;
  }

  write(filedesc, buffer, size);
  free(buffer);

  close(filedesc);

  rc = ssh_scp_pull_request(scp);
  if (rc != SSH_SCP_REQUEST_EOF)
  {
    fprintf(stderr, "Unexpected request: %s\n",
            ssh_get_error(session));
    return SSH_ERROR;
  }

  return SSH_OK;
}
int scp_helloworld(ssh_session session, ssh_scp scp)
{
  int rc;
  const char *helloworld = "Hello, world!\n";
  int length = strlen(helloworld);
  rc = ssh_scp_push_directory(scp, "helloworld", S_IRWXU);
  if (rc != SSH_OK)
  {
    fprintf(stderr, "Can't create remote directory: %s\n",
            ssh_get_error(session));
    return rc;
  }
  rc = ssh_scp_push_file
    (scp, "helloworld.txt", length, S_IRUSR |  S_IWUSR);
  if (rc != SSH_OK)
  {
    fprintf(stderr, "Can't open remote file: %s\n",
            ssh_get_error(session));
    return rc;
  }
  rc = ssh_scp_write(scp, helloworld, length);
  if (rc != SSH_OK)
  {
    fprintf(stderr, "Can't write to remote file: %s\n",
            ssh_get_error(session));
    return rc;
  }
  return SSH_OK;
}

int scp_write(ssh_session session)
{
  ssh_scp scp;
  int rc;
  scp = ssh_scp_new
    (session, SSH_SCP_WRITE | SSH_SCP_RECURSIVE, ".");
  if (scp == NULL)
  {
    fprintf(stderr, "Error allocating scp session: %s\n",
            ssh_get_error(session));
    return SSH_ERROR;
  }
  rc = ssh_scp_init(scp);
  if (rc != SSH_OK)
  {
    fprintf(stderr, "Error initializing scp session: %s\n",
            ssh_get_error(session));
    ssh_scp_free(scp);
    return rc;
  }
 //...
     scp_helloworld(session,scp);
  ssh_scp_close(scp);
  ssh_scp_free(scp);
  return SSH_OK;
}

int scp_read(ssh_session session)
{
  ssh_scp scp;
  int rc;
  scp = ssh_scp_new
    (session, SSH_SCP_READ, "./elif");
  if (scp == NULL)
  {
    fprintf(stderr, "Error allocating scp session: %s\n",
            ssh_get_error(session));
    return SSH_ERROR;
  }
  rc = ssh_scp_init(scp);
  if (rc != SSH_OK)
  {
    fprintf(stderr, "Error initializing scp session: %s\n",
            ssh_get_error(session));
    ssh_scp_free(scp);
    return rc;
  }
  scp_receive(session,scp);
  printf("dosya okundu..\n");
 // ...
  ssh_scp_close(scp);
  ssh_scp_free(scp);
  return SSH_OK;
}
/*int interactive_shell_open(ssh_channel channel)
{
  int rc;
  char buffer[256];
  int nbytes;
  rc = ssh_channel_request_pty(channel);
  if (rc != SSH_OK) return rc;
  rc = ssh_channel_change_pty_size(channel, 80, 24);
  if (rc != SSH_OK) return rc;
  rc = ssh_channel_request_shell(channel);
  if (rc != SSH_OK) return rc;
  while (ssh_channel_is_open(channel) &&
         !ssh_channel_is_eof(channel))
  {
    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    if (nbytes < 0)
      return SSH_ERROR;
    if (nbytes > 0)
      write(1, buffer, nbytes);
  }
  return rc;
}*/
/* Under Linux, this function determines whether a key has been pressed.
   Under Windows, it is a standard function, so you need not redefine it.
*/
int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}
/* A very simple terminal emulator:
   - print data received from the remote computer
   - send keyboard input to the remote computer
*/

int interactive_shell_open(ssh_channel channel)
{
  int rc;
  char buffer[256];
  int nbytes, nwritten;
  rc = ssh_channel_request_pty(channel);
  if (rc != SSH_OK) return rc;
  rc = ssh_channel_change_pty_size(channel, 80, 24);
  if (rc != SSH_OK) return rc;
  rc = ssh_channel_request_shell(channel);
  if (rc != SSH_OK) return rc;
  /* Session and terminal initialization skipped */


  while (ssh_channel_is_open(channel) &&
         !ssh_channel_is_eof(channel))
  {
    nbytes = ssh_channel_read_nonblocking(channel, buffer, sizeof(buffer), 0);
    if (nbytes < 0) return SSH_ERROR;
    if (nbytes > 0)
    {
      nwritten = write(1, buffer, nbytes);
      if (nwritten != nbytes) return SSH_ERROR;
    if (!kbhit())
    {
      usleep(50000L); // 0.05 second
      continue;
    }
    nbytes = read(0, buffer, sizeof(buffer));
    if (nbytes < 0) return SSH_ERROR;
    if (nbytes > 0)
    {
      nwritten = ssh_channel_write(channel, buffer, nbytes);
      if (nwritten != nbytes) return SSH_ERROR;
    }
  }
  }
  return rc;


}
ssh_channel open_channel(ssh_session session)
{
  ssh_channel channel;
  int rc;
  channel = ssh_channel_new(session);
  if (channel == NULL)
    return NULL;
  rc = ssh_channel_open_session(channel);
  if (rc != SSH_OK)
  {
    ssh_channel_free(channel);
    
    return NULL;
  }
 /* ssh_channel_close(channel);
  ssh_channel_send_eof(channel);
  ssh_channel_free(channel);
  */
  return channel;
}

void close_channel(ssh_channel channel)
{
  ssh_channel_close(channel);
  ssh_channel_send_eof(channel);
  ssh_channel_free(channel);
}


int show_remote_processes(ssh_session session,char *command)
{
  ssh_channel channel;
  int rc;
  char buffer[256];
  int nbytes;
  //char *command="ls";
  channel = ssh_channel_new(session);
  if (channel == NULL)
    return SSH_ERROR;
  rc = ssh_channel_open_session(channel);
  if (rc != SSH_OK)
  {
    ssh_channel_free(channel);
    return rc;
  }
  rc = ssh_channel_request_exec(channel, command);
  if (rc != SSH_OK)
  {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return rc;
  }
  nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  while (nbytes > 0)
  {
    if (write(1, buffer, nbytes) != (unsigned int) nbytes)
    {
      ssh_channel_close(channel);
      ssh_channel_free(channel);
      return SSH_ERROR;
    }
    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  }
  if (nbytes < 0)
  {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return SSH_ERROR;
  }
  ssh_channel_send_eof(channel);
  ssh_channel_close(channel);
  ssh_channel_free(channel);
  return SSH_OK;
}

/*****************************************************************/
int main()
{ 
  ssh_session my_ssh_session;
  int verbosity = SSH_LOG_PROTOCOL;
  int port = 22;
  int rc;
  char *password;
  // Open session and set options
  my_ssh_session = ssh_new();
  if (my_ssh_session == NULL)
    exit(-1);
  ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, "192.168.1.103");
  ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, "by");
  ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
 // ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);

     
  // Connect to server
  rc = ssh_connect(my_ssh_session);
  if (rc != SSH_OK)
  {
    fprintf(stderr, "Error connecting to localhost: %s\n",
            ssh_get_error(my_ssh_session));
    ssh_free(my_ssh_session);
    exit(-1);
  }
  // Verify the server's identity
  // For the source code of verify_knownhost(), check previous example
 /* if (verify_knownhost(my_ssh_session) < 0)
  {
    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
    exit(-1);
  }*/
  // Authenticate ourselves
 ///password = getpass("Password: ");
  password="1";
  rc = ssh_userauth_password(my_ssh_session, NULL, password);
  if (rc != SSH_AUTH_SUCCESS)
  {
    fprintf(stderr, "Error authenticating with password: %s\n",
            ssh_get_error(my_ssh_session));
    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
    exit(-1);
  }
  /*****************************************************/
 // scp_write(my_ssh_session);
 //scp_read(my_ssh_session);
 sftp_helloworld(my_ssh_session);
//show_remote_processes(my_ssh_session,"echo $DISPLAY");
//show_remote_processes(my_ssh_session,"xcalc&");
/*ssh_channel chanel1=open_channel(my_ssh_session);
if(chanel1!=NULL)
{
interactive_shell_open(chanel1);
}

close_channel(chanel1);*/
/***************************************************/

  ssh_disconnect(my_ssh_session);
  ssh_free(my_ssh_session);
}
