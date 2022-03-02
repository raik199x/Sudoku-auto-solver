#include <iostream>	
#include <cstdio>
#include <Windows.h>
using namespace std;
//copy our problem from a file
void fill_in(int (&pole)[9][9]) {
	FILE *txt;
	fopen_s(&txt, "pole.txt", "r+");
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			fscanf_s(txt, "%d", &pole[i][j]);
		}
	}
	fclose(txt);
} 
//output our solve
void output(int(&pole)[9][9]) {
	int boom = 0, bang = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			boom++;
			cout << pole[i][j] << " ";
			if (boom == 3) {
				boom -= 3;
				cout << " ";
			}
		}
		bang++;
		cout << endl;
		if (bang == 3) {
			bang -= 3;
			cout << endl;
		}
	}
}

int set_gran(int i) {
	if (i == 1)
		return 3;
	else if (i == 2)
		return 6;
	else
		return 9;
}
//checks if we can place the number depending on its location
int check_access(int(&pole)[9][9],int enter,int i, int j) {
	//checks access on line
	for (int z_j = 0; z_j < 9; z_j++) {
		if (pole[i][z_j] == enter)
			return 1;
	}
	//checks acces on table
	for (int z_i = 0; z_i < 9; z_i++) {
		if (pole[z_i][j] == enter)
			return 1;
	}
	int z_i, z_j; //check location

	//try to find out location for i
	if (i >= 0 && i < 3)
		z_i = 1;
	else if (i >= 3 && i < 6)
		z_i = 2;
	else
		z_i = 3;
//try to find our location for j
	if (j >= 0 && j < 3)
		z_j = 1;
	else if (j >= 3 && j < 6)
		z_j = 2;
	else
		z_j = 3;
	//setting our limits
	int gran_i = set_gran(z_i), gran_j = set_gran(z_j);
	//checking access of 3*3 cube
	for (z_i = gran_i-3; z_i < gran_i;z_i++)
		for (z_j = gran_j-3; z_j < gran_j; z_j++) {
			if (pole[z_i][z_j] == enter)
				return 1;
		}
	return 0;
}
//increasing our position
void check_increase(int &i, int &j) {
	j++;
	if (j > 8) {
		j -= 9;
		i++;
	}
}
//decrease position
void check_minus(int &i, int &j) {
	j--;
	if (j < 0) {
		j = 8;
		i--;
	}
}

void remove(int(&pole)[9][9], int &i, int &j) {
	check_minus(i, j);
	if (i < 0 || j < 0) {
		return;
	}
	int placeble = pole[i][j];
	pole[i][j] = 0;
	//but here come one thing, we dunno if that number place by our task or not, so i got an idea
   // i'll open file and find current position in it, if it place with not 0, it means we should skip our number
	FILE *find; // the thing is i should find the place where it stand
	//i would create formula to make it more easy to find place, but.. there is a problem with \n
	//sometimes it cound count and my numbers take a random place, i could guess how to avoid it but....
	fopen_s(&find, "pole.txt", "r+");
	char temp = ','; int n = 0;
	//first i found a table
	while (n != i || feof(find)) {
		fscanf_s(find, "%c", &temp);
		if (temp == '\n')
			n++;
	}
	n = 0;
	//second, i find a number, its easy, because here on a line could be only on number (that contain placeble)
	temp = '1'; int destiny = 0;
	while (temp != '\n') {
		fscanf_s(find, "%c", &temp);
		if (temp >= '0' && temp <= '9')
			n = temp - '0';
		else
			continue;
		if (n == placeble) {
			destiny = 1;
			break;
		}
	}
	if (destiny != 0) {
		pole[i][j] = placeble;
		fclose(find);   //do not forge to close file
		remove(pole, i, j);
		return;
	}
	placeble++;
	//long story short
	while (placeble <= 9) {
		int option = check_access(pole, placeble, i, j);  //check if we could place
		if (option != 0) { //if we couldnt - increase
			placeble++;
			continue;
		}
		pole[i][j] = placeble;
		fclose(find);  //dont forget to close our file
		return;
	}
	//if we come her, sad story become, so we go back in our function
	fclose(find); //dont forget to close our file
	remove(pole, i, j);
	return; //and maybe we finally exit
}

void calculating(int(&pole)[9][9]) {
	int i = 0, j = 0;


	while (i < 9) {
		if (i < 0 || j < 0) {
			cout << "this problem is unsolveable!" << endl;
			return;
		}
		int placeable = 1;
		//if number is already placed (by the task)
		if (pole[i][j] != 0) {
			check_increase(i, j);
			continue; //ingore this position
		}
		while (placeable <= 9) { //trying to brute number
			int option = check_access(pole, placeable, i, j);
			if (option == 0)
				break;
			placeable++;
		}
		if (placeable != 10) {
			pole[i][j] = placeable;
			check_increase(i, j);
			continue;
		}
		//here comes removing part
	   // because our past things doesnt go well, we try to increase past number, if we couldnt, we should delete it
		remove(pole, i, j);
	}
}

void regenerate() {
	FILE *problem;
	if (!fopen_s(&problem, "pole.txt", "w+")) {
		//done
	}
	else {
		cout << "fail to create file!" << endl;
		return;
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			fprintf(problem, "0");
			if (j == 8) {
				fprintf(problem, "\n");
				continue;
			}
			fprintf(problem, " ");
		}
	}
	fclose(problem);
}

int main() {
	int pole[9][9], option;
	FILE *check;
	if (fopen_s(&check, "pole.txt", "r+")) {
		regenerate();
	}
	else {
		fclose(check);
	}
	cout << "If you first time here, check your current locatin, here must be a file named pole.txt, enter here your problem" << endl;
	cout << "This program solve sudoku only 9*9, if you wrote another sudoku in a file, author do not garantee this promgram to work properly!" << endl;
	cout << "Feel free to use this program and enjoy yourself =)" << endl;
	system("pause");
	while (1) {
		system("cls");
		cout << "1. Regenerate problem file" << endl << "2. Solve the problem" << endl << "e. Exit" << endl;
		cin >> option;
		switch (option) {
		case 1: regenerate(); break;
		case 2: fill_in(pole);
			calculating(pole);
			output(pole);
			system("pause");
			break;
		default: return 0;
		}
	}
}