/*
 * --------------------------------------------------------
 * 
 * Aufgabe: Game Of Life
 * Autor: Thorsten Paßfeld
 * 
 * Ein Programm, welches das bekannte Game Of Life auf einer
 * AxB Ebene simuliert, wobei eine Datei im Life 1.06 Format
 * oder eine Zahl, mit der das Anfangsfeld zufallsgeneriert
 * wird, eingelesen werden kann.
 * 
 * Praktikum Grundlagen Programmierung, HS Osnabrueck
 * --------------------------------------------------------
 */

#include <cstdlib>
#include <iostream>
#include <limits>
#include <fstream>
#include <vector>
#include <iomanip>
#include "windows.h"

using namespace std;

/*
 *  Funktion: validInputInt
 * 
 *  Prueft eine Eingabe vom Nutzer darauf, ob sie tatsächlich
 *  vom Typ int, sowie groesser als 0 ist und verlangt sonst eine weitere Eingabe.
 *  Der gepruefte Wert wird dann ausgegeben.
 * 
 *  return: int -- Die gepruefte, bereinigte Eingabe
 * 
 */

int validInputInt() {
    int eingabe;

    cin >> eingabe;
    while (cin.fail() || eingabe < 1) //Solange die Eingabe nicht eindeutig von Typ int ist (Sonderzeichen, etc.)
    {
        cin.clear(); //Fehlerstatus wird auf goodbit gesetzt, um weiterhin mit der Eingabe zu arbeiten
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Eingabe wird bis zum newline character gelöscht
        cout << "Fehlerhafte Eingabe. Bitte eine gueltige Zahl eingeben: ";
        cin >> eingabe;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return eingabe;
}

/*
 *  Funktion: validInputWahrscheinlichkeit
 * 
 *  Prueft eine Eingabe vom Nutzer darauf, ob sie tatsächlich
 *  vom Typ int, sowie zwischen 0 und 100 liegt und verlangt sonst eine weitere Eingabe.
 *  Der gepruefte Wert wird dann ausgegeben.
 * 
 *  return: int -- Die gepruefte, bereinigte Eingabe
 * 
 */

int validInputWahrscheinlichkeit() {
    int eingabe;

    cin >> eingabe;
    while (cin.fail() || eingabe < 0 || eingabe > 100) //Solange die Eingabe nicht eindeutig von Typ int ist (Sonderzeichen, etc.)
    {
        cin.clear(); //Fehlerstatus wird auf goodbit gesetzt, um weiterhin mit der Eingabe zu arbeiten
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Eingabe wird bis zum newline character gelöscht
        cout << "Fehlerhafte Eingabe. Bitte eine gueltige Zahl zwischen 0 und 100 eingeben: ";
        cin >> eingabe;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return eingabe;
}

/*
 *  Funktion: validInputBool
 * 
 *  Prueft eine Eingabe vom Nutzer darauf, ob sie tatsächlich
 *  vom Typ bool ist und verlangt sonst eine weitere Eingabe.
 *  Der gepruefte Wert wird dann ausgegeben.
 * 
 *  return: bool -- Die gepruefte, bereinigte Eingabe
 * 
 */

bool validInputBool() {
    bool eingabe;

    cin >> eingabe;
    while (cin.fail()) //Solange die Eingabe nicht eindeutig von Typ int ist (Sonderzeichen, etc.)
    {
        cin.clear(); //Fehlerstatus wird auf goodbit gesetzt, um weiterhin mit der Eingabe zu arbeiten
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Eingabe wird bis zum newline character gelöscht
        cout << "Fehlerhafte Eingabe. Bitte 0 oder 1 eingeben: ";
        cin >> eingabe;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return eingabe;
}

/*
 *  Funktion: setValidFileName
 * 
 *  Stellt sicher, dass der eingegebene Dateiname auch tatsaechlich
 *  zu einer existierenden Datei gehoert.
 * 
 */

void setValidFileName(ifstream& inputStream, string& dateiname) {
    while (!inputStream) {
        inputStream.close();
        cout << "Bitte einen korrekten Dateinamen eingeben, z.B. game_of_life.txt: ";
        cin >> dateiname;
        cout << endl;
        inputStream.open(dateiname.c_str(), ios::in);
    }
    inputStream.close();
}

/*
 *  Funktion: readFile
 * 
 *  Liest eine existierende Datei im Life 1.06 Format und
 *  fuellt ihren Inhalt in einen Vektor, falls ihr Inhalt
 *  auch wirklich nur vom Typ Integer ist.
 * 
 */

void readFile(string& dateiname, vector<short int>& aliveCells) {

    ifstream inputStream(dateiname);
    setValidFileName(inputStream, dateiname);
    inputStream.open(dateiname.c_str(), ios::in);
    inputStream.clear();
    int zahl = 0;
    while (!inputStream.eof()) {
        inputStream >> zahl;
        if (inputStream.fail()) {
            cout << "Datei fehlerhaft. " << endl;
            inputStream.close();
            setValidFileName(inputStream, dateiname);
            readFile(dateiname, aliveCells);
        } else {
            aliveCells.push_back(zahl);
        }
    }
    inputStream.close();
}

/*
 *  Funktion: draw2dVec
 * 
 *  Gibt einen zweidimensionalen Vektor in der Konsole aus.
 * 
 */

void draw2dVec(const vector<vector<bool> >& grid, const bool& modus) {
    for (int x = 0; x < grid.size(); x++) {
        for (int y = 0; y < grid[0].size(); y++) {
            if (modus == 1) {
                if (grid[x][y] == 1) {
                    cout << "#";
                } else {
                    cout << " ";
                }
            }
            else if (modus == 0)
            {
                cout << grid[x][y];
            }
        }
        cout << endl;
    }
}

/*
 *  Funktion: generateRandomGrid
 * 
 *  Befuellt einen zweidimensionalen Vektor mit einer 1
 *  an einer bestimmten Stelle, wenn eine berechnete
 *  Zufallszahl groesser als die eingegebene Wahrscheinlichkeit ist.
 * 
 */

void generateRandomGrid(vector<vector<bool> >& grid, int& wahrscheinlichkeit) {
    int zufallszahl;
    for (int x = 1; x < grid.size()-1; x++) {
        for (int y = 1; y < grid[0].size()-1; y++) {
            zufallszahl = (rand() + 1) % 101;
            if (zufallszahl >= wahrscheinlichkeit) {
                grid[x][y] = 1;
            }
        }
    }
}

/*
 *  Funktion: setStartConfig
 * 
 *  Setzt an einer von der eingelesenen Datei im Life 1.06 Format
 *  bestimmten Stelle X Y eine 1, falls diese Stelle im zweidimensionalen Vektor
 *  liegt, welcher eine vom Nutzer bestimmte Anzahl an Zeilen und Spalten hat.
 * 
 */

void setStartConfig(vector<vector<bool> >&grid, const vector<short int>& aliveCells) {
    for (int i = 0; i < aliveCells.size(); i += 2) {
        if (aliveCells[i] < grid[0].size()-2 && aliveCells[i + 1] < grid.size()-2) {
            grid[aliveCells[i + 1]+1][aliveCells[i]+1] = 1;
        }
    }
}

/*
 *  Funktion: createEmpty2dVec
 * 
 *  Befuellt einen leeren zweidimensionalen Vektor mit Nullen.
 * 
 *  return grid : bool -- Ein zweidimensionaler Vektor mit 0 an jeder Stelle
 * 
 */

vector<vector<bool>> createEmpty2dVec(const int& zeilen, const int& spalten) {
    vector<bool> tempZahlen;
    vector < vector<bool>> grid;
    for (int j = 0; j < spalten+2; j++) {
        tempZahlen.push_back(0);
    }
    for (int i = 0; i < zeilen+2; i++) {
        grid.push_back(tempZahlen);
    }
    return grid;
}

/*
 *  Funktion: nextGeneration
 * 
 *  Der eigentliche Algorithmus vom Game Of Life. Ermittelt von jedem Feld
 *  die jeweilige Anzahl an lebendigen Nachbarn(1) und setzt ihren naechsten Zustand
 *  nach festgelegten Regeln.
 * 
 */

void nextGeneration(vector<vector<bool> >& grid, vector<vector<bool> >&nextGrid, bool& noChange) {

    int nachbarn = 0;

    for (int x = 1; x < grid.size()-1; x++) {
        for (int y = 1; y < grid[0].size()-1; y++) {
            nachbarn = 0;
            for (int dy = -1; dy <= 1; dy++) {
                //if (y + dy < 0 || y + dy >= grid[0].size()) continue;
                for (int dx = -1; dx <= 1; dx++) {
                    //if (x + dx < 0 || x + dx >= grid.size()) continue;
                    nachbarn += grid[x + dx][y + dy];
                }
            }
            //Den urspruenglichen Wert im Punkt abziehen, da man ihn oben mit addiert hat.
            nachbarn -= grid[x][y];

            //Neue Zustaende nach den festgelegten Regeln
            if (grid[x][y] == 1 && (nachbarn == 2 || nachbarn == 3)) nextGrid[x][y] = 1; //Stabile Bevoelkerung
            else if (grid[x][y] == 1 && (nachbarn < 2 || nachbarn > 3)) nextGrid[x][y] = 0; //Vereinsamung/Ueberbevoelkerung
            else if (grid[x][y] == 0 && nachbarn == 3) nextGrid[x][y] = 1; //Reproduktion
            else nextGrid[x][y] = grid[x][y]; //Stasis
        }
    }
    if (grid == nextGrid) noChange = true;
    grid = nextGrid;
}

/*
 *  Funktion: drawGameOfLife
 * 
 *  Animiert die Iterationen des Game Of Life Algorithmus und gibt die Dimensionen
 *  des Feldes, sowie die Anzahl der Iterationen bisher an. Gestoppt wird, wenn
 *  es zu einem Stillstand der Bevoelkerung kommt, bzw. wenn diese ausstirbt/ueberall
 *  stabil ist oder wenn die maximale Anzahl an Iterationen erreicht wurde..
 * 
 */

void playGameOfLife(vector<vector<bool> >& grid,vector<vector<bool> >& nextGrid, const int& maxIterationen, bool& modus) {
    bool noChange = false;
    int iterationen = 0;
    do {
        
        system("cmd /c cls");
        iterationen++;
        cout << "Dimensionen: " << grid[0].size() << " x " << grid.size() << endl
             << iterationen << ". Iteration von " << maxIterationen << endl;
        nextGeneration(grid, nextGrid, noChange);
        draw2dVec(nextGrid, modus);
        Sleep(200);
    } while (!noChange && maxIterationen != iterationen);
}

int main() {

    srand(time(0));
    system("cmd /c color F2");

    int zeilen, spalten, maxIterationen;
    string dateiname;
    cout << "Dieses Programm simuliert Lebenszyklen von Populationen auf einer 2-dimensionalen Ebene." << endl
            << "Bitte folgende Werte eingeben..." << endl << endl
            << "Zeilen: ";
    zeilen = validInputInt();
    cout << "Spalten: ";
    spalten = validInputInt();
    cout << "Anzahl der Iterationen: ";
    maxIterationen = validInputInt();

    vector < vector<bool>> grid = createEmpty2dVec(zeilen, spalten);
    vector < vector<bool>> nextGrid = createEmpty2dVec(zeilen, spalten);
    
    //draw2dVec(grid, 0);

    cout << endl << "Wollen sie eine Datei einlesen oder ein neues Spielfeld generieren?" << endl
            << "Datei einlesen <0>" << " oder " << "Neues Spielfeld generieren <1>" << endl;
    bool auswahl = validInputBool();

    if (auswahl == 0) {
        cout << "Name einer Datei im Life 1.06 Format (z.B. game_of_life.txt): ";
        cin >> dateiname;
        cout << endl;
        vector<short int> aliveCells;
        readFile(dateiname, aliveCells);
        setStartConfig(grid, aliveCells);
    } else if (auswahl == 1) {
        cout << "Geben sie eine ganze Zahl zwischen 0 und 100 an, als Wahrscheinlichkeit, dass eine beliebige Stelle XY 0 ist." << endl;
        int wahrscheinlichkeit = validInputWahrscheinlichkeit();
        generateRandomGrid(grid, wahrscheinlichkeit);
    }
    
    cout << endl << "Wie soll das Game Of Life dargestellt werden?" << endl
            << "<0> (1 = Lebendig; 0 = tot)" << " oder " << "<1> (# = lebendig, Leerzeichen = tot)" << endl;
    bool modus = validInputBool();

    //draw2dVec(grid, modus);
    //cout << endl << endl;
    playGameOfLife(grid, nextGrid, maxIterationen, modus);

    return 0;
}