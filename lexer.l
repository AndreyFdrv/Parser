%{
#include<stdio.h>
#include<string.h>
int colno=1, lineno=1;
int filter=0;
char lexems[1000]="\0";
%}

%%
[/][/][^\n]*[\n] {
if(filter==1)
{
colno=1;
lineno=lineno+1;
}
else
{
int i;
char text[yyleng];
for(i=2; i<yyleng; i++)
{
	text[i-2]=yytext[i];
}
text[yyleng-3]='\0';
char temp[100];
sprintf(temp, "Comment(\"%s\", %d, %d, %d); ", text, lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=1;
lineno=lineno+1;
}
}
[(][*]((([^*])*([^)])*)|((([^*])*([^)])*[*][^)]+[)]([^*])*([^)])*))*)[*][)] {
if(filter==1)
{
	int i;
	for(i=0; i<yyleng; i++)
  	{
		colno=colno+1;
		if(yytext[i]=='\n')
		{
			colno=1;
			lineno=lineno+1;
		}
	}
}
else
{
int i;
int begin_lineno=lineno;
int begin_colno=colno;
int end_colno;
for(i=0; i<yyleng; i++)
{
	end_colno=colno;
	colno=colno+1;
	if(yytext[i]=='\n')
	{
		colno=1;
		lineno=lineno+1;
	}
}
char text[yyleng];
for(i=2; i<yyleng; i++)
{
	text[i-2]=yytext[i];
}
text[yyleng-4]='\0';
char temp[100];
sprintf(temp, "Comment(\"%s\", %d, %d, %d, %d); ", text, begin_lineno, lineno, begin_colno, end_colno);
strcat(lexems, temp);
}
}
[*][*] {
char temp[100];
sprintf(temp, "Op(\"**\", %d, %d, %d); ", lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;
}
[\n] {colno=1;
lineno=lineno+1;}
[ ] {colno=colno+1;}
[\t] {colno=colno+1;} 
[s][k][i][p] {
char temp[100];
sprintf(temp, "KW_Skip(%d, %d, %d); ", lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
[:][=] {
char temp[100];
sprintf(temp, "Assignment(%d, %d, %d); ", lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
[\;] {
char temp[100];
sprintf(temp, "Colon(%d, %d, %d); ", lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
[w][r][i][t][e] {
char temp[100];
sprintf(temp, "KW_Write(%d, %d, %d); ", lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
[r][e][a][d] {
char temp[100];
sprintf(temp, "KW_Read(%d, %d, %d); ", lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
[w][h][i][l][e] {
char temp[100];
sprintf(temp, "KW_While(%d, %d, %d); ", lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
[d][o] {
char temp[100];
sprintf(temp, "KW_Do(%d, %d, %d); ", lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
[i][f] {
char temp[100];
sprintf(temp, "KW_If(%d, %d, %d); ", lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
[t][h][e][n] {
char temp[100];
sprintf(temp, "KW_Then(%d, %d, %d); ", lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
[e][l][s][e] {
char temp[100];
sprintf(temp, "KW_Else(%d, %d, %d); ", lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
[a-zA-Z_][a-zA-Z_0-9]* {yytext [yyleng] = (char) 0;
char temp[100];
sprintf(temp, "Var(\"%s\", %d, %d, %d); ", yytext, lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
([+|\-|/|%|>|<])|([=|\!][=])|([>|<][=])|([&][&])|([\|][\|]) {yytext [yyleng] = (char) 0;
char temp[100];
sprintf(temp, "Op(\"%s\", %d, %d, %d); ", yytext, lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
([*]) {
char temp[100];
sprintf(temp, "Op(\"%s\", %d, %d, %d); ", yytext, lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
[1-9][0-9]* {yytext [yyleng] = (char) 0;
char temp[100];
sprintf(temp, "Num(\"%s\", %d, %d, %d); ", yytext, lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
[\(] {yytext [yyleng] = (char) 0;
char temp[100];
sprintf(temp, "Left_Parenth(%d, %d, %d); ", lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
[\)] {yytext [yyleng] = (char) 0;
char temp[100];
sprintf(temp, "Right_Parenth(%d, %d, %d); ", lineno, colno, colno-1+yyleng);
strcat(lexems, temp);
colno=colno+yyleng;}
[^ \r\n] {
char temp[100];
sprintf(temp, "Error in line %d, columns %d - %d: \"%s\", lexer has been stoped", lineno, colno, colno-1+yyleng, yytext);
strcat(lexems, temp);
return;}
%%

void run_lexer(int argc,char *argv[], char *output)
{
filter=1;
yyin=fopen(argv[1],"r");
if(yyin==NULL)
{
	strcpy(output, "File does not exist\n");
	return;
}	
else
{
	yylex();
}
strcpy(output, lexems);
}

