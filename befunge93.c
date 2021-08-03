//--------Anastasios Diamantis ------
//-------Programming Languages 2-----
//-----Virtual Machine for Befunge-93

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>


typedef signed long int i32;
struct pc{
	i32 x;
	i32 y;
	i32 size_x;
	i32 size_y;
};

void left(struct pc* pc){
	if(pc->y == 0){
		pc->y = pc->size_y - 1;
	}else{
		pc->y--;
	}
}

void right(struct pc* pc){
	if(pc->y == pc->size_y - 1){
		pc->y = 0;
	}else{
		pc->y++;
	}
}

void down(struct pc* pc){
	if(pc->x == pc->size_x - 1){
		pc->x = 0;
	}else{
		pc->x++;
	}
}

void up(struct pc* pc){
	if(pc->x == 0){
		pc->x = pc->size_x - 1;
	}else{
		pc->x--;
	}
}

int main(int argc,char **argv){
	FILE *inFile;
	char c;
	i32* stack = (i32*) malloc(sizeof(int) * (pow(2,20)));
	i32 top = 0,N = 0 ,K = 0,command,temp,x,y,val,randomdir;
	size_t len;
	char *line = NULL;
	char grid[25][80];
	stack[top] = 0;
	inFile = fopen(argv[1],"r");
	if(inFile == NULL) {
		exit(0);
	}else{
		i32 i = -1;		
		while(getline(&line,&len,inFile) != -1){
			++i;
			K = strlen(line) ;
			for(i32 j = 0;j < K;j++){
				grid[i][j] = line[j];
			}
			
		}
		N = i+1;
	}
	struct pc pc = {0,0,N,K};
	fclose(inFile);

	//ASCII table of commands -> labels
	static void *jumpto[129];
	for(int i = 0;i <129; i++){
		jumpto[i] = &&exit;
	}
	jumpto[43] = &&add;
	jumpto[45] = &&subtract;
	jumpto[42] = &&multiply;
	jumpto[47] = &&divide;
	jumpto[37] = &&modulo;
	jumpto[33] = &&not;
	jumpto[96] = &&greater;
	jumpto[60] = &&ch_left;
	jumpto[62] = &&ch_right;
	jumpto[94] = &&ch_up;
	jumpto[118] = &&ch_down;
	jumpto[63] = &&random;
	jumpto[95] = &&horif;
	jumpto[124] = &&verif;
	jumpto[34] = &&stringmode;
	jumpto[58] = &&dup;
	jumpto[92] = &&swap;
	jumpto[36] = &&pop;
	jumpto[46] = &&outputint;
	jumpto[44] = &&outputchar;
	jumpto[35] = &&bridge;
	jumpto[103] = &&get;
	jumpto[112] = &&put;
	jumpto[38] = &&inputint;
	jumpto[126] = &&inputcharacter;
	jumpto[64] = &&exit;
	jumpto[32] = &&right;
	void *direction;
	direction = &&right;
begin:	
		command = grid[pc.x][pc.y];
		if (command >=48 && command <=57){
			top++;
			stack[top] = command - 48;
			goto *direction;
		}
		
		if(command == 32){
			goto *direction;
		}
		goto *jumpto[command];
ch_up:
	direction = &&up;
	goto up;

up:
	up(&pc);
	goto begin;
	
ch_down:
	direction = &&down;
	goto down;

down:
	down(&pc);
	goto begin;

ch_left:
	direction = &&left; 
	goto left;

left:
	left(&pc);
	goto begin;

ch_right:
	direction = &&right;
	goto right;

right:
	right(&pc);
	goto begin;

add: 
	if (top == 0){
		//nothing
	}else{
		top--;
		stack[top] = stack[top+1] + stack[top];
	}

	goto *direction;

subtract:
	if(top == 0){
		stack[1] = stack[0];
		stack[0] = 0;
		top = 1;
	}
	stack[top-1] = stack[top-1] - stack[top];
	top--;
	
	goto *direction;

multiply:
	if(top == 0){
		top = 0;
		stack[top] = 0;
	}else{
		--top;
		stack[top] = stack[top] * stack[top+1];
	}
	goto *direction;

divide:
	if(top == 0){
		top = 1;
		stack[1] = stack[0];
		stack[0] = 0;
	}
	--top;
	stack[top] = stack[top]/stack[top+1];
	goto *direction;

modulo:
	if(top == 0){
		stack[1] = stack[0];
		stack[0] = 0;			 
		top = 1;
	}
	top--;
	stack[top] = stack[top] % stack[top+1];
	goto *direction;

not:
	
	if(stack[top] == 0){
		stack[top] = 1;
	}else{
		stack[top] = 0;
	}
	goto *direction;

inputint:
	++top;
	scanf(" %li",&temp);
	stack[top] = temp;
	goto *direction;

inputcharacter:
	scanf(" %c", &c);
	++top;
	stack[top] = (i32) c;
	goto *direction;

dup:
	++top;
	stack[top] = stack[top-1];
	goto *direction;

swap:
	temp = stack[top];
	stack[top] = stack[top-1];
	stack[top - 1] = temp;
	goto *direction;
pop:
	if(top == 0){
		stack[0] = 0;
	}else{
		--top;
	}
	goto *direction;

bridge:
	if(direction == &&left){
		left(&pc);
	}else if(direction == &&right ){
		right(&pc);
	}else if(direction == &&up){
		up(&pc);
	}else{
		down(&pc);
	}
	goto *direction;

outputint:
	printf("%li ",stack[top]);
	--top;
	if(top == -1){
		stack[0] = 0;
		top = 0;
	}
	
	goto *direction;

outputchar:
	c = (char) stack[top];
	printf("%c",c);

	--top;
	if(top == -1){
		stack[0] = 0;
		top = 0;
	}

	goto *direction;

greater:
	if (top == 0){
		stack[1] = stack[0];
		stack[0] = 0;
		top = 1;
	}
	if (stack[top-1] > stack[top]){
		--top;
		stack[top] = 1;
	}else{
		--top;
		stack[top] = 0;
	}
	
	goto *direction;

horif:
	if(stack[top] == 0){
		if(top==0){
			//nothing
		}else{
			--top;
		}
		direction = &&right;
	}else{
		if(top==0){
			stack[top] = 0;
		}else{
			--top;
		}
		direction = &&left;
	}

	goto *direction;

verif:
	if(stack[top] == 0){
		if(top==0){
			//nothing
		}else{
			--top;
		}
		direction = &&down;
	}else{
		if(top==0){
			stack[top] = 0;
		}else{
			--top;
		}
		direction = &&up;
	}
	goto *direction;

random:
	randomdir = rand()% 4 ;
	if (randomdir == 0){
		direction = &&up;
	}else if(randomdir == 1){
		direction = &&right;
	}else if(randomdir == 2){
		direction = &&down;
	}else{
		direction = &&left;
	}
	goto *direction;

put:		
	if(top == 0){
		stack[2] = stack[0];
		top = 2;
		stack[0] = 0;
		stack[1] = 0;
	}else if (top == 1){
		stack[2] = stack[1];
		stack[1] = stack[0];
		stack[0] = 0;
		top = 2;
	}
	x = stack[top];
	y = stack[top-1];
	val = stack[top-2];
	top =- 2;
	if(top == -1){
		stack[0] = 0;
		top = 0;
	}
	if(val >=0 && val <=9){
		grid[x][y] =  (val + 48);
	}else{
		grid[x][y] = val;
	}
	goto *direction;

get:
	if(top == 0){
		stack[1] = stack[0];
		top = 1;
		stack[0] = 0;
	}
	command = (i32) grid[stack[top]][stack[top-1]];
	
	stack[top-1] = command;
	--top;
	goto *direction;

stringmode:
	if(direction == &&left){
		left(&pc);
	}else if(direction == &&right ){
		right(&pc);
	}else if(direction == &&up){
		up(&pc);
	}else{
		down(&pc);
	}
	
	while(grid[pc.x][pc.y] != '\"'){
		++top;
		stack[top] = (i32) grid[pc.x][pc.y];
		if(direction == &&left){
			left(&pc);
		}else if(direction == &&right ){
			right(&pc);
		}else if(direction == &&up){
			up(&pc);
		}else{
			down(&pc);
		}
	}
	goto *direction;

exit: 
	
	return 0;
}

