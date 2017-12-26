#include <stdio.h>
#include <stdlib.h>


double** multiply(double**, double**, int, int, int , int);
double** transpose(double**, int , int);
double** inverse(double**, int, int);
void freeArray(double**, int);



int main(int argc, char** argv)
{
FILE *trainfile = fopen(argv[1], "r");
FILE *testfile = fopen(argv[2], "r");

int attributes;
int numhouses;
int numtest;

fscanf(trainfile,"%d\n", &attributes);
fscanf(trainfile, "%d\n", &numhouses);
fscanf(testfile, "%d\n" , &numtest);


double** X = (double**)malloc(numhouses*sizeof(double*));
double** Y = (double**)malloc(numhouses*sizeof(double*));
int x,y;

//allocating memory for all the array spaces
//scanning in X and Y

for(x=0; x<numhouses; x++){
	X[x]= (double*)malloc((attributes+1)*sizeof(double));
}
for(x=0; x<numhouses; x++){
	Y[x]= (double*)malloc(1*sizeof(double));
}

for(x=0; x<numhouses; x++){
	X[x][0] = 1;
}

for(x=0; x<numhouses; x++){
	for(y=1; y< attributes+2; y++){
		if(y==(attributes+1)){
			fscanf(trainfile, "%lf\n" , &Y[x][0]);
		}
		else
			fscanf(trainfile, "%lf" , &X[x][y]);
		fscanf(trainfile, ",");
	}
}
attributes++;

double** first = (double**)malloc(attributes*sizeof(double*));
double** second = (double**)malloc(attributes*sizeof(double*));
double** weight = (double**)malloc(attributes*sizeof(double*));

for(x=0; x<attributes; x++){
	first[x]= (double*)malloc(attributes*sizeof(double));
	second[x]= (double*)malloc(1*sizeof(double));
	weight[x]= (double*)malloc(1*sizeof(double));
}
double** q = transpose(X, numhouses, attributes);
double** w = multiply(q,X, attributes, numhouses , numhouses, attributes);
first = inverse(w, attributes, attributes);
second = multiply(q, Y, attributes, numhouses, numhouses, 1);
weight = multiply(first, second, attributes, attributes, attributes, 1);

double firstw = weight[0][0];
double** newweight = (double**)malloc((attributes-1)*sizeof(double*));
for(x=0; x<(attributes-1); x++){
	newweight[x]=(double*)malloc(1*sizeof(double));
}

for(x=0; x<(attributes-1); x++){
	newweight[x][0]=weight[x+1][0];
}

/*int i,j;
for(i=0; i< (attributes-1); i++){
		printf("%1f\t",newweight[i][0]);
	printf("\n");
}
*/
attributes--;

double** test= (double**)malloc(numtest*sizeof(double*));;
for(x=0; x<numtest; x++){
		test[x]= (double*)malloc(attributes*sizeof(double));
}
for(x=0; x< numtest; x++){
	for(y=0; y< attributes; y++){
		fscanf(testfile, "%lf", &test[x][y]);
		fscanf(testfile, ",");
	}
	fscanf(testfile, "\n");
}

/*for(i=0; i< numtest; i++){
	for(j=0;j<attributes; j++){
		printf("%1f\t",test[i][j]);
	}
	printf("\n");
}
*/
double** prices =(double**)malloc(numtest*sizeof(double*));
for(x=0; x<numtest; x++){
	prices[x]= (double*)malloc(1* sizeof(double));
}

for(x=0; x< numtest; x++){
	for(y=0;y<attributes;y++){
		prices[x][0]+=(newweight[y][0]*test[x][y]);
		
	}
	prices[x][0]+=firstw;
}


for(x=0; x< numtest; x++){
	printf("%0.0lf\n", prices[x][0]);
}
freeArray(X,numhouses);
freeArray(Y,numhouses);
freeArray(q,attributes+1);
freeArray(w,attributes+1);
freeArray(first,attributes+1);
freeArray(second,attributes+1);
freeArray(weight,attributes+1);
freeArray(newweight,attributes);
freeArray(test,numtest);
freeArray(prices,numtest);

return 0;

}

double** multiply(double** arr1, double** arr2, int row1, int col1, int row2, int col2){
//Assume We Know the Sizes 

double** multiply = (double**)malloc(row1*sizeof(double*)); 
int x;
for(x=0; x<row1; x++){
	multiply[x]= (double*)malloc(col2 * sizeof(double));
}


int a,b,c;

for(a=0; a<row1; a++){
	for(b=0; b<col2; b++)
   {
      multiply[a][b] = 0;
   }
}
for(a = 0; a < row1; a++) {
   for (b = 0; b < col2; b++) {
      for (c = 0; c < col1; c++) {
       multiply[a][b]+= arr1[a][c]*arr2[c][b];
      }
   }
}
return multiply;
}
double** transpose(double** arr, int row, int col){

int a,b;
//switch rows and cols
double** transpose = (double**)malloc(col * sizeof(double*)); 
int x;
for(x=0; x<col; x++){
		transpose[x]= (double*)malloc(row * sizeof(double));
}


for(a=0; a<row; a++){
	for(b=0; b<col; b++){
		transpose[b][a] = arr[a][b];
	}
}
return transpose;
}

double** inverse( double** arr, int row, int col){
int newcol = col*2;
double** newMatrix = (double**)malloc(row*sizeof(double*)); 
int x,y,z;

for(x=0; x<row; x++){
	newMatrix[x]= (double*)malloc(newcol*sizeof(double));
}

for(x=0; x<row;x++){
	for(y=0;y<col;y++){
		newMatrix[x][y] = arr[x][y];
	}
}

for(x=0; x<row; x++){
	for(y=col; y <newcol; y++){
		if((x+col)==y)
			newMatrix[x][y] = 1;
		else
			newMatrix[x][y] = 0;
	}
}

//First Iteration to Create Upper Diagonal Matrix

for(x=0; x< row; x++){
	for(y=0; y< col; y++){
		if(x==y){
			if(newMatrix[x][y] != 1){
				double element = newMatrix[x][y];
				for(z = 0; z< newcol; z++){
					newMatrix[x][z] = newMatrix[x][z] / element;
				}
			}
			break;
		}
		else if(newMatrix[x][y] != 0){
				double element = newMatrix[x][y];
				for(z = 0; z< newcol; z++){
					newMatrix[x][z] = newMatrix[x][z] - (element* newMatrix[y][z]);
				}
		}
	}
}


//Second Iteration to Create Bottom Diagonal Matrix

for(x=row-2; x>=0; x--){
	for(y=x+1; y<col; y++){
		if(newMatrix[x][y] != 0){
			double element = newMatrix[x][y];
			for(z = 0; z< newcol; z++){
					newMatrix[x][z] = newMatrix[x][z] - (element* newMatrix[y][z]);
			}
		}
	}
}

double** inverse = (double**)malloc(row*sizeof(double*));
for(x=0; x<row; x++){
		inverse[x]= (double*)malloc(col * sizeof(double));
}
		
for(x=0; x<row; x++){
	for(y=0; y<col; y++){
			inverse[x][y]=newMatrix[x][y+col];
	}
}
freeArray(newMatrix,row);
return inverse;
}

void freeArray(double** a, int row) {
    int i;
    for (i = 0; i < row; i++) {
        free(a[i]);
    }
    free(a);
}































