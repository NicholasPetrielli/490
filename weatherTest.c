#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEF2STR(x) #x

int main(){
	system("curl \"http://api.openweathermap.org/data/2.5/weather?q=Toronto&APPID=279adcf563de661f5c8b7dd9dee187c5\" > file.txt");
	system("tr ',' '\n' < file.txt >file2.txt");
	system("tr -d '\"' < file2.txt > file.txt");
	system("tr -d '[' < file.txt > file2.txt");
	system("tr -d ']' < file2.txt > file.txt");
	system("tr -d '{' < file.txt > file2.txt");
	system("tr -d '}' < file2.txt > file.txt");
	system("tr ':' ' ' < file.txt > file2.txt");
	system("rm file.txt");
	FILE *fp;
  	char buff[255];
	char buffCopy[255];
	char commandBuffer[600];
	system("> weather.txt");

	fp = fopen("file2.txt", "r");

	while(fgets(buff, 255, (FILE*)fp)){
		strcpy(buffCopy,buff);
		int i = 0;
		while( i < 255){
		if(buffCopy[i] == '\n'){
			buffCopy[i] ='\0';
		}
		i++;
		}
		char* token = strtok(buff, " ");
		while(token){
		if 	  (strcmp(token, "description") == 0){
			sprintf(commandBuffer, "echo %s >> weather.txt", buffCopy);
			system(commandBuffer);
		} else if (strcmp(token, "humidity") == 0){
			sprintf(commandBuffer, "echo %s >> weather.txt", buffCopy);
                        system(commandBuffer);
		} else if (strcmp(token, "main") == 0){
			token = strtok(NULL, " ");
			if (strcmp(token, "temp") == 0){
				token = strtok(NULL, " ");
				float num = atof(token) - 273.15;
				sprintf(commandBuffer, "echo The temperature is %.1f degrees celsius >> weather.txt", num);
                        	system(commandBuffer);
			}
		}
		token = strtok(NULL, " ");
		}
	}
  	fclose(fp);
	return 1;
}
