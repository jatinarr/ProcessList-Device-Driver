#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#define MSG_SIZE 250

int main() {
	int fp,status;
	char msg[MSG_SIZE];  /*Buffer to hold the device read data.*/

	/*Open the Process_List character device in READ ONLY mode.*/
	fp = open("/dev/process_list",O_RDONLY);
	if(fp<0) {
		perror("ERROR in opening device Process_List");
		return 0;
	}

	/*Looping till the last process details. 
	If result of read is 0 then read completes.*/
	while((status = read(fp,msg,strlen(msg)))  > 0) {
		printf("%s\n",msg);
		memset(msg,0,sizeof(char)*MSG_SIZE);
	}
	if(status < 0) {
		perror("ERROR in reading from device Process_List!");
		return 0;
	}
	printf("\n");
	/*Close the Device*/
	close(fp);

	return 0;
}

