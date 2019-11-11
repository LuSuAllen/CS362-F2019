#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
    //0 to 93.  Adding 32 would bring it to 32(space) and 125(})
	char random = (rand() % 94) + 32;
    return random;
}

char *inputString()
{
	static char s[6];		//Array that will be returned
	int i = 0;

	//0 to 15.  Adding 101 would bring it to 101(e) and 116(t)
	while (i < 5)
	{
		s[i] = (rand() % 16) + 101;
		i++;
	}

    return s;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) //91
		state = 1;
    if (c == '(' && state == 1) //40
		state = 2;
    if (c == '{' && state == 2) //123
		state = 3;
    if (c == ' '&& state == 3)  //32
		state = 4;
    if (c == 'a' && state == 4) //97
		state = 5;
    if (c == 'x' && state == 5) //120
		state = 6;
    if (c == '}' && state == 6) //125
		state = 7;
    if (c == ')' && state == 7) //41
		state = 8;
    if (c == ']' && state == 8) //93
		state = 9;
    if (s[0] == 'r' && s[1] == 'e' && s[2] == 's' && s[3] == 'e' && s[4] == 't' && s[5] == '\0' && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
