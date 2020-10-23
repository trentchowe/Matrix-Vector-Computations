#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>
using namespace std;
#include "array_dim.h"

//prototype function that reads N and fills A and B
int readN(ifstream &inputFile, double[][DIM], double[]);
//prototype function to show off A and B
void showArrays(int, double[][DIM], double[], ofstream &outFile, double[]);
//prototype function that implements matrix-vector multiplication
void matrixVectorMultiply(double[][DIM], double[], double[]);
//prototype function that writes only first n rows and columns of A
void showOnlyNRowsColumns(double[][DIM], int, ofstream &outFile);
//prototype function that copies upper-left portion of one matrix to another
void copyUpperLeft(double[][DIM], double[][DIM-1], int, int, int);
//prototype function to get determinant of square matrix
double showDet(double[][DIM], int);
//prototype function to copy all but one of columns to another matrix
void copyColsButOne(double[][DIM],double[],double[][DIM],int);

int main()
{
	//declare matrix A and vector B
	double A[DIM][DIM], B[DIM];
	//open text file for output
	ofstream outFile;
	outFile.open("ECE0301_ICA06_Axeqb_solution.txt");
	//report N from input file
	ifstream inFile;
	inFile.open("ECE0301_ICA06_Axeqb_problem.txt");
	int N = readN(inFile, A, B);
	//declare i, j, n
	int n=DIM;
	//write first n rows and columns of A
	// showOnlyNRowsColumns(A, n, outFile);
	//call function to compute determinant of A
	double det = showDet(A,n);
	//display determinant
	// outFile <<"det(A) = "<<det<<endl; 
	//define column-replaced matrix
	double colReplaced[DIM][DIM];
	//loop over columns of A to get solution vector
	//define 1D array x
	double x[DIM];
	for (int i=0; i<DIM; i++)
	{
		//call colreplace function but copy b to current column i
		copyColsButOne(A,B,colReplaced,i);
		//call function from step 10
		// showOnlyNRowsColumns(colReplaced, n, outFile);
		//call function from step 12 to calculate determinant
		double detColReplaced = showDet(colReplaced, n);
		x[i] = detColReplaced/det;
		//outFile << "D = "<< detColReplaced << endl;
	}
							//declare new 2D array to receive copied matrix
							// double copiedMatrix[DIM][DIM];
							//call matrix copy function
							// copyUpperLeft(A, copiedMatrix, i, j, n);
							//call function to receive first n-1 rows of copied matrix
							// n--;
							// showOnlyNRowsColumns(copiedMatrix, n, outFile);
	
	//get user to input elements
	//for (int i=0; i<DIM; i++)
	//	{
		//	cout<<"Enter elements of x: ";
		//	cin>>x[i];
		//	cout<<endl;
	//	}
	//declare product vector Ax
	double Ax[DIM];
	//call matrix vector multiplication function
	matrixVectorMultiply(A, x, Ax);
	//call output function
	showArrays(N, A, B, outFile, x);
}

//function that reads N, A, and B
int readN(ifstream &inputFile, double A[][DIM], double B[])
{
	//each word piece by piece and then the number value
	string partOne, partTwo, partThree, partFour, partFive, partSix, partSeven, partEight;
	int valueN;
	inputFile >> partOne>>partTwo>>partThree>> partFour>> partFive>> partSix>> partSeven>>partEight>>valueN;
	//error check
	if (partOne != "ECE" || partTwo != "0301:" || partThree != "Ax" || partFour != "=" || partFive != "b" || partSix != "Problem" || partSeven !="N" || partEight != "=")
	{
		cout<<"ERROR! Input file format error.";
		return(-1);
	}
	else if(valueN != DIM)
	{
		cout<< "ERROR! Dimension mismatch, N != DIM.";
		return(-1);
	}
	else
	{
		//read also matrix a and b
		string partNine, partTen;
		//check formatting before A
		inputFile >> partNine >> partTen;
		if (partNine != "A" || partTen != "=")
			{
				cout<<"ERROR! Input file format error.";
				return(-1);
			}
			else
			{
				//read next N^2 lines and fill A array
				for (int i = 0; i < valueN; i++)
					{
						for (int j = 0; j < valueN; j++)
						{
							inputFile >> A[i][j];
						}
					}
				//check if next parts say b = 
				string partEleven, partTwelve;
				inputFile >> partEleven >> partTwelve;
				if (partEleven != "b" || partTwelve != "=")
					{
						cout<<"ERROR! Input file format error.";
						return(-1);
					}
				else
				{
					//read next N lines and fill b vector
					for (int i = 0; i<valueN; i++)
					{
						inputFile >> B[i];
					}
					//return N
					return(valueN);
				}
			}
	}
}

//function for info on Ax = b
void showArrays(int N, double A[][DIM], double B[], ofstream &outFile, double x[])
{
	//display stuff that used to be in main
	outFile<<"ECE 0301 - Matrix-Vector Computations,"<<endl<<"Determinants and Cramer's Rule."<<endl;
	outFile<<endl<<"Global array dimension: DIM = "<<DIM<<endl;
	outFile<<"Input file: N = "<<N<<endl<<endl;
	//display A as a square matrix
	outFile <<"A ="<<endl;
	for (int i = 0; i<N; i++)
	{
		for (int j = 0; j<N; j++)
		{
			if(j==0)
			{
				outFile<<"[";
			}
			outFile<<setw(10)<<A[i][j];
			if(j==N-1)
			{
				outFile<<" ]"<<endl;
			}
			
		}
	}
	//display B as a vector
	outFile<<endl<<"b = "<<endl;
	for (int i = 0; i<N; i++)
		{
			outFile<<"["<<setw(10)<<B[i]<<" ]"<<endl;
		}
	//write matrix-vector product Ax = b to the file
	outFile << endl << "Problem: A * x = b"<< endl;
	for (int i=0; i<N; i++)
	{
		for (int j=0; j<N; j++)
		{
			if(j==0)
			{
				outFile<<"[";
			}
				outFile<<setw(10)<<A[i][j];
		}
		outFile<<" ] ";
		//switch case for where the * goes (only making if i have to at the end)
		switch(DIM)
		{
		case 2:
				if(i==0)
				{
					outFile<<"*";
				}
				else
				{
					outFile<<" ";
				}
				break;
		case 3: 
				if(i==1)
				{
					outFile<<"*";
				}
				else
				{
					outFile<<" ";
				}
				break;
		case 4:
				if(i==1)
				{
					outFile<<"*";
				}
				else
				{
					outFile<<" ";
				}
				break;
		case 5:if(i==2)
				{
					outFile<<"*";
				}
				else
				{
					outFile<<" ";
				}
				break;
			}
		outFile<<" [ x"<<i<<" ] ";
		switch(DIM)
		{
		case 2:
				if(i==0)
				{
					outFile<<"=";
				}
				else
				{
					outFile<<" ";
				}
				break;
		case 3: 
				if(i==1)
				{
					outFile<<"=";
				}
				else
				{
					outFile<<" ";
				}
				break;
		case 4:
				if(i==1)
				{
					outFile<<"=";
				}
				else
				{
					outFile<<" ";
				}
				break;
		case 5:if(i==2)
				{
					outFile<<"=";
				}
				else
				{
					outFile<<" ";
				}
				break;
			}
		outFile<<" ["<<setw(10)<<B[i]<<" ]"<<endl;
	}
	//write solution vector x
	outFile<<endl<<endl<<"Solution: x = "<<endl;
	for (int i=0;i<DIM;i++)
		{
			outFile<<"["<<setw(10)<<x[i]<<" ]"<<endl;
		}
	//define array for Ax
	double Ax[DIM];
	//compute Ax
	matrixVectorMultiply(A,x,Ax);
	//output Ax = b solution
	outFile<<endl<<"Checking Solution: A * x = b"<<endl;
	for (int i=0; i<DIM; i++)
	{
		for (int j=0; j<DIM; j++)
		{
			if(j==0)
			{
				outFile<<"[";
			}
				outFile<<setw(10)<<A[i][j];
		}
		outFile<<" ] ";
		//switch case for where the * goes (only making if i have to at the end)
		//switch case for where the * goes (only making if i have to at the end)
		switch(DIM)
		{
		case 2:
				if(i==0)
				{
					outFile<<"*";
				}
				else
				{
					outFile<<" ";
				}
				break;
		case 3: 
				if(i==1)
				{
					outFile<<"*";
				}
				else
				{
					outFile<<" ";
				}
				break;
		case 4:
				if(i==1)
				{
					outFile<<"*";
				}
				else
				{
					outFile<<" ";
				}
				break;
		case 5:if(i==2)
				{
					outFile<<"*";
				}
				else
				{
					outFile<<" ";
				}
				break;
			}
		outFile<<" ["<<setw(10)<<x[i]<<" ] ";
		//switch case for where the * goes (only making if i have to at the end)
		switch(DIM)
		{
		case 2:
				if(i==0)
				{
					outFile<<"=";
				}
				else
				{
					outFile<<" ";
				}
				break;
		case 3: 
				if(i==1)
				{
					outFile<<"=";
				}
				else
				{
					outFile<<" ";
				}
				break;
		case 4:
				if(i==1)
				{
					outFile<<"=";
				}
				else
				{
					outFile<<" ";
				}
				break;
		case 5:if(i==2)
				{
					outFile<<"=";
				}
				else
				{
					outFile<<" ";
				}
				break;
			}
		outFile<<" ["<<setw(10)<<Ax[i]<<" ]"<<endl;
	}
	//compute and report error vector 
	double errorArray[DIM];
	for (int i=0;i<DIM;i++)
		{
			errorArray[i] = Ax[i] - B[i];
		}
	//display error vector
	outFile<<endl<<"Error in RHS values:"<<endl;
	for (int i=0;i<DIM;i++)
	{
		outFile<<"["<<setw(10)<<errorArray[i]<<" ]"<<endl;
	}
}

//function for matrix-vector multiplication
void matrixVectorMultiply(double A[][DIM], double x[], double Ax[])
{
	for (int i=0; i<DIM; i++)
		{
			for (int j=0; j<DIM; j++)
			{
				Ax[i] += (A[i][j] * x[j]);
			}
		}
}

//function displaying only first n rows/columns of A
void showOnlyNRowsColumns(double A[][DIM], int n, ofstream &outFile)
{
	for (int i=0; i<n; i++)
	{
		for (int j=0; j<n; j++)
		{
			if(j==0)
			{
				outFile<<"[";
			}
			outFile<<setw(10)<<A[i][j];
			if(j==n-1)
			{
				outFile<<" ]"<<endl;
			}
		}
	}
}

//function copying upperleft portion of matrix to another
void copyUpperLeft(double A[][DIM], double copiedMatrix[][DIM], int i, int j, int n)
{
	int a,b;
	a=0;
	for (int x = 0; x<n; x++)
		{
			if(x!=i){
				b=0;
			for (int y = 0; y<n; y++)
				{
					if (y!=(j))
						{
							copiedMatrix[a][b] = A[x][y];
							b++;
						}
						
				}
				a++;
			}
		}
}

//function getting determinant
double showDet(double A[][DIM], int n)
{
	double det=0;
	//case if n=1
	if (n==1)
	{
		det = A[0][0];
		return(det);
	}
	//define new 2D array
	double subMatrix[DIM][DIM];
	//loop over elemenets in row 0
	for (int x = 0; x<n; x++)
		{
			//call function from step 11 to copy first n rows and columns except row 0 and column of current element to first n-1 rows/cols of submatrix
			int i = 0;
			int j=x;
			copyUpperLeft(A, subMatrix, i, j, n);
			//recursively call determinant function
			double detAdd = showDet(subMatrix, (n-1));
			detAdd *= pow(-1, j);
			detAdd *= A[0][x];
			det += detAdd;
		}
		return(det);
}

//function copying all but one column
void copyColsButOne(double A[][DIM], double B[], double copiedCols[][DIM], int n)
{
	int a, b;
	a = 0;
	for (int i=0;i<DIM;i++)
		{
			b=0;
			for (int j=0; j<DIM; j++)
				{
					if(j!=n)
					{
						copiedCols[a][b] = A[i][j];
						
					}
					b++;
				}
		a++;
		}
	//replace empty column with b
	for (int k=0;k<DIM;k++)
		{
			copiedCols[k][n] = B[k];
		}
	
}
