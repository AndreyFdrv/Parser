#include <stdio.h>
#include <string.h>
extern void Step1(char *text);
extern int isTerminal(char* text, int i);
extern void Step2(char *text);
extern int isLeftBorder(char c);
extern int isRightBorder(char c);
extern void substr(char *str1, char *str2, int begin, int end);
extern void begin_substr(char *str1, char *str2, int end);
extern void end_substr(char *str1, char *str2, int begin);
extern void Step3(char *text);
extern int isRuleNew(char *text, char *rule, int right_part_begin);
extern void Step4(char *text);
extern void Step5(char *text);
extern void run_lexer(int argc,char *argv[], char *output);
int main(int argc, char *argv[])
{
	FILE* in=fopen("Grammar.txt","r");
	if(in==NULL)
		printf("File of grammar does not exist\n");
	else
	{	
		char lexems[1000];
		run_lexer(argc, argv, lexems);
		for(int i=0; lexems[i]!='\0'; i++)
		{
			if((lexems[i]=='E')&&(lexems[i+1]=='r')&&(lexems[i+2]=='r')&&(lexems[i+3]=='o')&&(lexems[i+4]=='r')&&(lexems[i+5]==' '))
			{
				end_substr(lexems, lexems, i);
				printf("%s\n", lexems);
				return 1;
			}
			if((lexems[i]=='F')&&(lexems[i+1]=='i')&&(lexems[i+2]=='l')&&(lexems[i+3]=='e')&&(lexems[i+4]==' '))
			{
				end_substr(lexems, lexems, i);
				printf("%s\n", lexems);
				return 1;
			}
		}
		char grammar[500] = "\0";
		char* str_err;
		char str[50];
		str_err = fgets(str, 50, in);
		while (str_err != NULL)
		{
			if (str[0] != 10)
			{
				strcat(grammar, str);
				str_err = fgets(str, 50, in);
			}
			else
				str_err = fgets(str, 50, in);
		}
		printf("The grammar after normalization:\n");
		printf("%s", grammar);
		int N = 0;
		for(int i=0; lexems[i]!='\0'; i++)
		{
			if(lexems[i]==';')
				N++;
		}
		int mas[40][40][40];
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				for (int k = 0; k < 40; k++)
					mas[i][j][k] = 0;
			}
		}
		char way_mas[40][40][40];
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				for (int k = 0; k < 40; k++)
					way_mas[i][j][0] = '\0';
			}
		}
		int mas_index=-1;
		for (int i = 0; lexems[i]!='\0'; i++)
		{
			char lexem[20];
			int j;
			for(j=i; lexems[j]!='('; j++);
			substr(lexem, lexems, i, j-1);
			while(lexems[i]!=';')
				i++;
			char info[50];
			substr(info, lexems, j, i-1);
			i++;
			mas_index++;
			char cur_nonterminal, left_nonterminal = grammar[0];
			int nonterminal_index, left_nonoterminal_index = 0;
			for (int k = 2; grammar[k] != '\0';)
			{
				int left_border, right_border;
				if (grammar[k] == '\n')
				{
					left_nonterminal = grammar[k + 1];
					left_nonoterminal_index = k + 1;
				}
				if (isLeftBorder(grammar[k]))
				{
					left_border = k;
					k++;
					while (!isRightBorder(grammar[k]))
					{
						right_border = k + 1;
						k++;
					}
					char right_part[50];
					substr(right_part, grammar, left_border + 1, right_border - 1);
					if (!strcmp(right_part, lexem))
					{
						if (left_nonterminal=='S')
							mas[mas_index][mas_index][0] = 1;
						else
							mas[mas_index][mas_index][left_nonterminal - 'A' + 1] = 1;
						char left[2];
						left[0]=left_nonterminal;
						left[1]='\0';
						strcat(way_mas[mas_index][mas_index], left);
						strcat(way_mas[mas_index][mas_index], "->");
						strcat(way_mas[mas_index][mas_index], lexem);
						strcat(way_mas[mas_index][mas_index], info);
						strcat(way_mas[mas_index][mas_index], "\n");
					}
				}
				else
					k++;
			}
		}
		for (int diag = 1; diag < N; diag++)
		{
			for (int i = 0; i < N - diag; i++)
			{
				for (int l = i; l < i + diag; l++)
				{
					for (int k = 0; k < 40; k++)
					{
						if (mas[i][l][k])
						{
							for (int ik = 0; ik < 40; ik++)
							{
								if (mas[l+1][i + diag][ik])
								{
									char new_pair[3];
									if (k == 0)
										new_pair[0] = 'S';
									else if(k==26)
										new_pair[0] = 'a';
									else if(k>18)
										new_pair[0] = 'A' + k;
									else
										new_pair[0] = 'A' + k - 1;
									if (ik == 0)
										new_pair[1] = 'S';
									else if(ik==26)
										new_pair[1] = 'a';
									else if(ik>18)
										new_pair[1] = 'A' + ik;
									else
										new_pair[1] = 'A' + ik - 1;
									new_pair[2] = '\0';
									char cur_nonterminal, left_nonterminal = 'S';
									int nonterminal_index, left_nonoterminal_index = 0;
									for (int m = 2; grammar[m] != '\0';)
									{
										int left_border, right_border;
										if (grammar[m] == '\n')
										{
											left_nonterminal = grammar[m + 1];
											left_nonoterminal_index = m + 1;
										}
										if (isLeftBorder(grammar[m]))
										{
											left_border = m;
											m++;
											while (!isRightBorder(grammar[m]))
											{
												right_border = m + 1;
												m++;
											}
											char right_part[50];
											substr(right_part, grammar, left_border + 1, right_border - 1);
											if (!strcmp(right_part, new_pair))
										        {
												if (left_nonterminal == 'S')
													mas[i][i + diag][0] = 1;
												else if (left_nonterminal == 'a')
													mas[i][i + diag][26] = 1;
												else if (left_nonterminal > 'R')
													mas[i][i + diag][left_nonterminal - 'A'] = 1;
												else
													mas[i][i + diag][left_nonterminal - 'A' + 1] = 1;
												char left[2];
												left[0]=left_nonterminal;
												left[1]='\0';
												strcat(way_mas[i][i+diag], left);
												strcat(way_mas[i][i+diag], "->");
												strcat(way_mas[i][i+diag], new_pair);
												strcat(way_mas[i][i+diag], "\n");
											}
										}
										else
											m++;
									}
								}
							}
						}
					}
				}
			}
		}
		if (mas[0][N - 1][0])
		{
			printf("The program is correct\n");
			printf("Output tree:\n");
			char tree[500] = "S\n";
	int i, j;
	for (i = 0; way_mas[0][N - 1][i] != '\0'; i++)
	{
		if ((way_mas[0][N - 1][i] == 'S') && (way_mas[0][N - 1][i + 1] == '-'))
		{
			for (j = i; way_mas[0][N - 1][j] != '\n'; j++);
			break;
		}
	}
	char right[50];
	if ((j - i == 5) && (way_mas[0][N - 1][i + 3] >= 'A') && (way_mas[0][N - 1][i + 3] <= 'Z') && (way_mas[0][N - 1][i + 4] != 'p'))
	{
		char first[5];
		first[0] = way_mas[0][N - 1][i + 3];
		first[1] = ' ';
		first[2] = '\0';
		substr(right, way_mas[0][N - 1], i + 4, j);
		strcat(first, right);
		strcpy(right, first);
	}
	else
		substr(right, way_mas[0][N - 1], i + 3, j);
	strcat(tree, right);
	int begin = 2;
	int end;
	for (end = begin; tree[end] != '\n'; end++);
	end--;
	int cur_i[50], cur_j[50];
	for (int i = 0; i<50; i++)
	{
		cur_i[i] = -1;
		cur_j[i] = -1;
	}
	cur_i[0] = 0;
	cur_j[0] = N - 1;
	int isNonterminal = 1;
	while (isNonterminal)
	{
		char new_cur_i[50], new_cur_j[50];
		for (int i = 0; i<50; i++)
		{
			new_cur_i[i] = -1;
			new_cur_j[i] = -1;
		}
		int length = -1, cur_i_index = 0, cur_j_index = 0;
		int new_cur_i_index = 0, new_cur_j_index = 0;
		isNonterminal = 0;
		for (int tree_i = begin; tree_i <= end; tree_i++)
		{
			length++;
			if (tree[tree_i] == ' ')
			{
				if (length == 1)
				{
					isNonterminal = 1;
					char cur_nonterminal = tree[tree_i - 1];
					int ki, kj, isFound = 0, j;
					for (j = 0; j<cur_j[cur_j_index]; j++)
					{
						for (ki = 0; way_mas[cur_i[cur_i_index]][j][ki] != '\0'; ki++)
						{
							if ((way_mas[cur_i[cur_i_index]][j][ki] == cur_nonterminal) && (way_mas[cur_i[cur_i_index]][j][ki + 1] == '-'))
							{
								for (kj = ki; way_mas[cur_i[cur_i_index]][j][kj] != '\n'; kj++);
								isFound = 1;
								break;
							}
						}
						if (isFound)
							break;
					}
					if ((kj - ki == 5) && (way_mas[cur_i[cur_i_index]][j][ki + 3] >= 'A') && (way_mas[cur_i[cur_i_index]][j][ki + 3] <= 'Z') && (way_mas[cur_i[cur_i_index]][j][ki + 4] != 'p'))
					{
						char first[5];
						first[0] = way_mas[cur_i[cur_i_index]][j][ki + 3];
						first[1] = ' ';
						first[2] = '\0';
						substr(right, way_mas[cur_i[cur_i_index]][j], ki + 4, kj - 1);
						strcat(first, right);
						strcpy(right, first);
						strcat(right, " ");
						new_cur_i[new_cur_i_index] = cur_i[cur_i_index];
						new_cur_j[new_cur_j_index] = j;
						new_cur_i_index++;
						new_cur_j_index++;
					}
					else
					{
						substr(right, way_mas[cur_i[cur_i_index]][j], ki + 3, kj - 1);
						strcat(right, " ");
					}
					strcat(tree, right);
					cur_nonterminal = tree[tree_i + 1];
					isFound = 0;
					int i;
					for (i = cur_i[cur_i_index] + 1; i<N; i++)
					{
						for (ki = 0; way_mas[i][cur_j[cur_j_index]][ki] != '\0'; ki++)
						{
							if ((way_mas[i][cur_j[cur_j_index]][ki] == cur_nonterminal) && (way_mas[i][cur_j[cur_j_index]][ki + 1] == '-'))
							{
								for (kj = ki; way_mas[i][cur_j[cur_j_index]][kj] != '\n'; kj++);
								isFound = 1;
								break;
							}
						}
						if (isFound)
							break;
					}
					char right[50];
					if ((kj - ki == 5) && (way_mas[i][cur_j[cur_j_index]][ki + 3] >= 'A') && (way_mas[i][cur_j[cur_j_index]][ki + 3] <= 'Z') && (way_mas[cur_i[cur_i_index]][j][ki + 4] != 'p'))
					{
						char first[5];
						first[0] = way_mas[i][cur_j[cur_j_index]][ki + 3];
						first[1] = ' ';
						first[2] = '\0';
						substr(right, way_mas[i][cur_j[cur_j_index]], ki + 4, kj - 1);
						strcat(first, right);
						strcpy(right, first);
						strcat(right, " ");
						new_cur_i[new_cur_i_index] = i;
						new_cur_j[new_cur_j_index] = cur_j[cur_j_index];
						new_cur_i_index++;
						new_cur_j_index++;
					}
					else
					{
						substr(right, way_mas[i][cur_j[cur_j_index]], ki + 3, kj - 1);
						strcat(right, " ");
					}
					strcat(tree, right);
					tree_i += 3;
					cur_i_index++;
					cur_j_index++;
				}
				length = -1;
			}
		}
		begin = end + 2;
		for (end = begin; tree[end] != '\0'; end++);
		end--;
		tree[end] = '\n';
		end--;
		for (int i = 0; i<50; i++)
		{
			cur_i[i] = new_cur_i[i];
			cur_j[i] = new_cur_j[i];
		}
	}
	char tree1[10000]="                       S\n";
	int cur_pos = 0, prev_pos = 0, tree_i = 2;
		begin = 0;
	while (!((tree1[begin] == '\n') && (tree1[begin-1] == '\n')))
	{
		int i;
		char next_str[500] = "\0", str1[500]="\0", str2[500]="\0", str3[500]="\0";
		cur_pos = 0;
		for (i = begin; tree1[i] != '\n'; i++)
		{
			cur_pos++;
			if (tree1[i] != ' ')
			{
				if ((tree1[i + 1] == ' ') || (tree1[i + 1] == '\n') || (tree1[i + 1] == '\0'))
				{
					int spaces = cur_pos-1;
					for (int j = 0; str1[j] != '\0'; j++)
					{
						spaces--;
					}
					for (int j = 0; j < spaces; j++)
					{
						strcat(str1, " ");
					}
					strcat(str1, "|");
					while ((tree[tree_i] == ' ') || (tree[tree_i] == '\n'))
						tree_i++;
					if ((tree[tree_i + 1] == ' ') || (tree[tree_i + 1] == '\n') || (tree[tree_i + 1] == '\0'))
					{
						int delta = cur_pos - prev_pos;
						int spaces = prev_pos + delta - delta / 2;
						int str2_begin = spaces;
						for (int j = 0; next_str[j] != '\0'; j++)
						{
							spaces--;
						}
						for (int j = 0; j < spaces; j++)
						{
							strcat(next_str, " ");
						}
						char symbol[2];
						symbol[0] = tree[tree_i];
						symbol[1] = '\0';
						tree_i++;
						strcat(next_str, symbol);
						tree_i++;
						symbol[0] = tree[tree_i];
						symbol[1] = '\0';
						tree_i++;
						spaces = prev_pos + delta + delta / 2;
						int str2_end = spaces;
						int str2_spaces = str2_begin;
						int str3_spaces = str2_spaces;
						for (int j = 0; str2[j] != '\0'; j++)
						{
							str2_spaces--;
						}
						for (int j = 0; j < str2_spaces; j++)
						{
							strcat(str2, " ");
						}
						for (int j = str2_begin; j <= str2_end; j++)
						{
							strcat(str2, "-");
						}
						for (int j = 0; str3[j] != '\0'; j++)
						{
							str3_spaces--;
						}
						for (int j = 0; j < str3_spaces; j++)
						{
							strcat(str3, " ");
						}
						strcat(str3, "|");
						for (int j = str2_begin; j <= str2_end-2; j++)
						{
							strcat(str3, " ");
						}
						strcat(str3, "|");
						for (int j = 0; next_str[j] != '\0'; j++)
						{
							spaces--;
						}
						for (int j = 0; j < spaces; j++)
						{
							strcat(next_str, " ");
						}
						strcat(next_str, symbol);
					}
					else
					{
						int spaces = cur_pos-8;
						int str2_spaces = cur_pos - 1;
						for (int j = 0; str2[j] != '\0'; j++)
						{
							str2_spaces--;
						}
						for (int j = 0; j < str2_spaces; j++)
						{
							strcat(str2, " ");
						}
						strcat(str2, "|");
						int str3_spaces = cur_pos - 1;
						for (int j = 0; str3[j] != '\0'; j++)
						{
							str3_spaces--;
						}
						for (int j = 0; j < str3_spaces; j++)
						{
							strcat(str3, " ");
						}
						strcat(str3, "|");
						for (int j = 0; next_str[j] != '\0'; j++)
						{
							spaces--;
						}
						for (int j = 0; j < spaces; j++)
						{
							strcat(next_str, " ");
						}
						char lexem[50];
						while ((tree[tree_i] == ' ') || (tree[tree_i] == '\n'))
							tree_i++;
						int tree_j;
						for (tree_j = tree_i; tree[tree_j]!=')'; tree_j++);
						substr(lexem, tree, tree_i, tree_j);
						strcat(next_str, lexem);
						tree_i = tree_j+1;
					}
					prev_pos = cur_pos;
				}
				else
				{
					while (tree1[i] != ')')
					{
						i++;
						cur_pos++;
					}
					prev_pos = cur_pos;
				}
			}
		}
		strcat(str1, " \n");
		strcat(str2, " \n");
		strcat(str3, " \n");
		strcat(next_str, "\n");
		strcat(tree1, str1);
		strcat(tree1, str2);
		strcat(tree1, str3);
		strcat(tree1, next_str);
		strcpy(str1, "\0");
		strcpy(str2, "\0");
		strcpy(str3, "\0");
		strcpy(next_str, "\0");
		for (int index = 0; index < 4; index++)
		{
			begin = i + 1;
			if ((tree1[begin] == '\n') && (tree1[begin - 1] == '\n'))
				break;
			for (i = begin; tree1[i] != '\n'; i++);
		}
	}
	tree1[strlen(tree1)-1] = '\0';
	printf("%s", tree1);
		}
		else
			printf("Syntactic error! Program is incorrect\n");
	}
}
