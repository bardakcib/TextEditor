#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define Max_Statement_Size 40
#define lastnode -1
#define inputsize 256
#define deletenode -2

struct node 
{
	int 	statno;	// statement number
	char 	statement[40];  // max. 40 characters
	int 	next;  	// points to the textbuffer[] index of the next statement 
};
struct dfs 
{
	int 	code;		// 1-insertion 2-deletion 0-otherwise
	int	statno;	
	char 	statement[40];  // only for insertion
}; 
struct dfs diffs[20]; // max. 20 changes
int version=0;	// version number
struct node textbuffer[30]; // max. 30 lines
int head;
int buffercount = 0; //counts textbuffer elements
char I[1]={'I'}; //for menu checking
char D[1]={'D'}; //for menu checking
char S[1]={'S'}; //for menu checking
char E[1]={'E'}; //for menu checking
char P[1]={'P'}; //for menu checking
char C[1]={'C'}; //for menu checking
char X[1]={'X'}; //for menu checking
char option[1]; // taking menu input
char filename[Max_Statement_Size]; //inpufilename
int minstatno=999; //for
int maxstatno;
char tmpdiffilename[40]={".dif"};
char diffilename[40];//create dif file name
int diffscounter=0; //difference counter
int difversion; //checking difference version
int difcode; //checkingdifferencecode
int difstatno; //checking difference statno
char difstate[Max_Statement_Size]; //difference statement 

void insert(int statno, char *stat,int diff){
	//finding first statno and last statno
	for(int i=0; i<buffercount; i++){
		if(textbuffer[i].statno < minstatno){
			minstatno = textbuffer[i].statno;
		}
		if(textbuffer[i].statno > maxstatno){
			maxstatno = textbuffer[i].statno;
		}
	}
	//when we edit file again not to add difference array
	if(diff == 1){
		diffs[diffscounter].code = 1;
		diffs[diffscounter].statno =statno;
		strcpy(diffs[diffscounter].statement,stat);
		diffscounter++;
	}
	//adding head
	if(statno < minstatno){
		textbuffer[buffercount].next = head;
		head = buffercount;
		textbuffer[buffercount].statno = statno;
		strcpy(textbuffer[buffercount].statement,stat);
		buffercount++;
	}
	//adding lastnode
	else if(statno > maxstatno){
		for(int i = 0; i<buffercount; i++){
			if(textbuffer[i].statno == maxstatno){
				textbuffer[i].next = buffercount;
				textbuffer[buffercount].next = lastnode;
				strcpy(textbuffer[buffercount].statement,stat);
				textbuffer[buffercount].statno = statno;
				buffercount++;
				break;
			}
		}
	}
	//adding between
	else if(statno > minstatno && statno < maxstatno){
		for(int i=0; i<buffercount; i++){
			if(textbuffer[i].statno < statno && statno < textbuffer[textbuffer[i].next].statno){
				textbuffer[buffercount].next = textbuffer[i].next;
				textbuffer[i].next = buffercount;
				textbuffer[buffercount].statno = statno;
				strcpy(textbuffer[buffercount].statement,stat);
				buffercount++;
			}
		}	
	}
}

void delete(int statno,int diff){
	for(int i=0; i<buffercount; i++){
		if(textbuffer[i].statno < minstatno){
			minstatno = textbuffer[i].statno;
		}
		if(textbuffer[i].statno > maxstatno){
			maxstatno = textbuffer[i].statno;
		}
	}
	//when we edit file again not to add difference array
	if(diff == 1){
		diffs[diffscounter].code = 2;
		diffs[diffscounter].statno =statno;
		diffscounter++;
	}
	//delete head
	if(statno == minstatno){
		for(int i=0; i<buffercount; i++){
			if(textbuffer[i].statno == statno){
				head = textbuffer[i].next;
				textbuffer[i].next = deletenode;
			}
		}
	}
	//delete last
	else if(statno == maxstatno){
		for(int i =0; i<buffercount; i++){
			if(textbuffer[textbuffer[i].next].statno == statno){
				textbuffer[textbuffer[i].next].next = deletenode;
				textbuffer[i].next = lastnode;
			}
		}
	}
	//delete between
	else if(statno > minstatno && statno < maxstatno){
		for(int i=0; i<buffercount; i++){
			if(textbuffer[textbuffer[i].next].statno == statno){
				textbuffer[i].next = textbuffer[textbuffer[i].next].next;
			}
		}
	}

}

void edit(char *filename,int userversion){
	char input[256];
	char prestate[Max_Statement_Size];
	char *token;
	char *token2;
	int statNo;
	int checkversion;
	int control;
	buffercount = 0;
	FILE *openfile;
	openfile = fopen(filename,"r");
	if(openfile != NULL){
		while(fgets(input,inputsize,openfile)){
			if(strlen(input) != 0){
				input[strcspn(input, "\r\n")] = 0;
				token = strtok(input," ");
				statNo = atoi(token);
				token = strtok(NULL,"\n");
				if(token != NULL){
					strcpy(textbuffer[buffercount].statement,token);
				}
				textbuffer[buffercount].statno = statNo;
				textbuffer[buffercount].next = buffercount +1;
				buffercount++;
			}
		}
		if(feof(openfile)){
			textbuffer[buffercount-1].next = lastnode;
		}
		fclose(openfile);
	}
	else{
		printf("File cant open.\n");
	}
	// for every differense
	if(userversion == 0){
		FILE *editfile;
		editfile = fopen(diffilename,"r");
		if(editfile != NULL){
			while(fgets(input,inputsize,editfile)){
				if(strlen(input) != 0){
					input[strcspn(input, "\r\n")] = 0;
					token = strtok(input," ");
					if(token != NULL){
						checkversion = atoi(token);
						control = checkversion;
					}
					token = strtok(NULL, " ");
					if(token != NULL){
						difcode = control;
				
						difstatno = atoi(token);
						
						token = strtok(NULL, "\0");
						if(token != NULL){
							strcpy(difstate,token);
							
						}
						if(difcode == 1){
							insert(difstatno,difstate,0);
			
						}
							
						}
						if(difcode == 2){
							delete(difstatno,0);
						}
					else{
						if(checkversion>0){
							difversion = checkversion;
						}
					}
					
				}
			}
			fclose(editfile);
		}
		else{
			printf("File cant open\n");
		}	
	}
	// if input specific version
	else if(userversion > 0){
		FILE *editfile;
		editfile = fopen(diffilename,"r");
		if(editfile != NULL){
			while(fgets(input,inputsize,editfile)){
				if(strlen(input) != 0){
					input[strcspn(input, "\r\n")] = 0;
					token = strtok(input," ");
					if(token != NULL){
						checkversion = atoi(token);
						control = checkversion;
					}
					if(difversion == userversion && checkversion == -1){
						break;
					}
					token = strtok(NULL, " ");
					if(token != NULL){
						difcode = control;
						difstatno = atoi(token);
						token = strtok(NULL, "\0");
						if(token != NULL){
							strcpy(difstate,token);
							
						}
						if(difcode == 1){
							insert(difstatno,difstate,0);
						}
						else if(difcode == 2){
							delete(difstatno,0);
						}
					}
					else{
						if(checkversion>0){
							difversion = checkversion;
							
						}
					}
					
				}
			}
			fclose(editfile);
		}		
		else{
			printf("File cant open\n");
		}
	}
}

void save(){
	int check = head;
	FILE *savefile;
	if(version == 0){
		savefile = fopen(diffilename,"w");
		version++;
		if(savefile != NULL){
			fprintf(savefile,"%d\n",version);
			for(int i=0; i<diffscounter; i++){
				fprintf(savefile,"%d %d %s\n",diffs[i].code,diffs[i].statno,diffs[i].statement);
				
			}
			fprintf(savefile,"%d\n",-1);
			diffscounter=0;
		}
		else{
			printf("Cant opened\n");
		}		
		fclose(savefile);
	}
	else if(version >= 1){
		savefile = fopen(diffilename, "a");
		if(savefile != NULL){
			version++;
			fprintf(savefile,"%d\n",version);
			for(int i=0; i<diffscounter; i++){
				fprintf(savefile,"%d ",diffs[i].code);
				if(diffs[i].code == 2){
					fprintf(savefile,"%d\n",diffs[i].statno);
					
				}
				else{
					fprintf(savefile,"%d %s\n",diffs[i].statno,diffs[i].statement);
					
				}
				
			}
			fprintf(savefile,"%d\n",-1);
			diffscounter=0;
			
		}
		else{
			printf("Cant opened\n");
		}
		fclose(savefile);
	}
}


void commit(){
	int check = head;
	FILE *savefile;
	savefile = fopen(filename,"w");
	if(savefile != NULL){
		while(true){
			fprintf(savefile,"%d %s\n",textbuffer[check].statno,textbuffer[check].statement);
			if(textbuffer[check].next != lastnode){
				check = textbuffer[check].next;
			}
			else{
				break;
			}
		}
		
	}
	else{
		printf("Cant opened\n");
	}		
	fclose(savefile);
	savefile=fopen(diffilename,"w");
	if(savefile != NULL){
		fprintf(savefile,"0");
		version = 0;
	}
	fclose(savefile);
}
void print(){
   int check = head;
    while(true){
        printf("StatNo:%d\n",textbuffer[check].statno);
        printf("Statement:%s\n",textbuffer[check].statement);
        printf("Next:%d\n",textbuffer[check].next);
        if(textbuffer[check].next != lastnode){
            check = textbuffer[check].next;
        }else{
            break;
        }
    }
}

int main(){
	
	char input[256];
	char tmp;
	char *token;
	int userversion=-1;
	printf("Enter Filename:");
	scanf("%s",filename);
	scanf("%c",&tmp);
	edit(filename,-1);
	token = strtok(filename,".");
	strcpy(diffilename,token);
	strcat(diffilename,tmpdiffilename);
	FILE *diffile;
	diffile = fopen(diffilename ,"w");
	if(diffile != NULL){
		fprintf(diffile,"0");
		fclose(diffile);
		version = 0;
	}
	while(option[0] != X[0]){
		userversion = 0;
		printf("I-For Insert\nD-For Delete\nP-For Print\nS-For Save\nE-For Edit\nC-For Commit\nX-For Exit\n");
		printf("Enter Command:");
		scanf("%[^\n]",input);
		scanf("%c",&tmp);
		token = strtok(input," ");
		strcpy(option,token);
		token = strtok(NULL, " ");
		if(token != NULL){
			strcpy(filename,token);
		}
		token = strtok (NULL, "\n");
		if(token != NULL){
			userversion = atoi(token);
		}
		if(option[0] == I[0]){
			int userstatno;
			char prestate[Max_Statement_Size];
			printf("Enter Statement No:");
			scanf("%d",&userstatno);
			scanf("%c",&tmp);
			printf("Enter Statement:");
			scanf("%[^\n]",prestate);
			scanf("%c",&tmp);
			insert(userstatno,prestate,1);
		}
		else if(option[0] == D[0]){
			int userstatno;
			printf("Enter Statement No For Delete:");
			scanf("%d",&userstatno);
			scanf("%c",&tmp);
			delete(userstatno,1);
		}
		else if(option[0] == S[0]){
			save();
		}
		else if(option[0] == P[0]){
			print();
		}
		else if(option[0] == C[0]){
			commit();
		}
		else if(option[0] == E[0]){
			edit(filename,userversion);
		}	
	}
	return 0;
}














