/* Build by Johnsmz */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdbool.h>
#define null NULL

typedef void* element;
typedef char* string;

int err=0;
int LARGE_INT=8000000;
long long LARGE_LONGLONG=1000000000000000000;
extern int errno;

void print(string print){printf("%s",print);}
double power(double base,double exp){if(exp<0){exp*=-1;base=1/base;}int out = 1;for(int i = 0; i < exp;i++){out*=base;}return out;}
double absolute(double in){if(in<0){return -in;}return in;}
string clone_string(string str){char out[strlen(str)];strcpy(out, str);string ret = out; return ret;}
string substring(string str, int start, int end){if(start<0||end>(int)strlen(str)||start>=end){return "Error";}char out[end-start];for (int i = 0; i < end-start; i++){out[i]=str[i+start];}string ret = out;return ret;}
string scan(void){string out = malloc(sizeof(char)*LARGE_INT);fgets(out, LARGE_INT, stdin);for(int i = strlen(out)-1;;i--){if(out[i]=='\n'){out[i]=0;break;}}return out;}
int digit(int in){if(in==0){return 1;};int n = 0; while(in>0){n+=1;in/=10;}return n;}
int length(string str){return strlen(str);}
char itc(int in){if(in<0||in>9){return ' ';}return (char)(in+48);}
string ita(int in){int a = digit(in);char arr[a];for(int i = 0; i < a; i++){arr[a-i-1]=itc(in%10);in/=10;}string ret = arr;return ret;}
string fta(double in, int dec){if(dec<0){return "";}if((int)(in*power(10.0,dec+1.0))%10>=5){in+=power(10.0, dec*(-1.0));}int a = digit((int)in);char out[dec+1+a];string temp = ita((int)in);for(int i = 0; i < a; i++){out[i]=temp[i];};out[a]='.';for(int i = 1; i <=dec; i++){in*=10;out[a+i]=itc((int)in%10);}string ret = out;return ret;}
int transferNum(char a){if((int)a>=97&&(int)a<=122){return (int)a-87;}return (int)a-48;}
char transferChar(int a){if(a>9){return (char)(a+87);}else if(a>=0){return (char)(a+48);}return ' ';}
bool equals(string ar1,string ar2){if(strcmp(ar1, ar2)==0){return 1;}return 0;}
void Error_Print(void){printf("%s\n",strerror(errno));}
string readLine(FILE*file){char str[LARGE_INT];int i = 0;while(1){char f = fgetc(file);if(f=='\n'){break;}str[i]=f;i++;}string out=str;return out;}
string readFile(FILE*file){char str[LARGE_INT];int i = 0;while(1){char f = fgetc(file);if(f==EOF){break;}str[i]=f;i++;}string out=str;return out;}
bool str__equal(string a, string b, int len){for(int i = 0; i < len; i++){if(a[i]!=b[i]){return 0;}}return 1;}
void rand_s(void){srand(time(NULL)); rand();}
double randd(void) {return (double)rand() / RAND_MAX;}
