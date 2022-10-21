/*                                      
Name: Nathan George
Lab #10
Date: March 27, 2021
Description: A clean and veratile Tic Tac Toe game in the console.
Section: 50
*/


#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <winuser.h>

#include <iostream>
#include <string>
#include <vector>
using namespace std;

void repeatChar(int num, string toRepeat) {
    for (int i = 0; i < num; i++){
        cout << toRepeat;
    }
}

string GetKeyboardInput(){
    int charGet = 224;
    while(charGet==224){
        charGet = 0;
        charGet = _getch();
        switch(charGet) 
        {
            case 72:
                return "up";
                break;
            case 80:
                return "down";
                break;
            case 75:
                return "left";  // key left
                break;
            case 77:
                return "right";  // key right
                break;
            case 27:
                return "esc";
            case 224:
                break;
            default:
                string c(1, (char)charGet);
                return c;
                break;
        }
    }
    return "null";
}

template <typename T>
class List
{
    public:
    vector<T> data;

    List(){
        data.resize(0);
    }

    List(vector<T> initialData){
        data = initialData;
    }

    void Add(T item){
        data.push_back(item);
    }

    void Insert(T item, int index){
        data.insert(data.begin() + index, item);
    }

    void ReplaceAt(int index, T item){
        data[index] = item;
    }

    void RemoveDuplicates(){
        for (size_t i = 0; i < data.size(); i++){
            for (size_t j = 0; j < data.size(); j++){
                if(data[i] == data[j] && i != j){
                    RemoveAt(i);
                    i--;
                    j--;
                }
            }
        }
    }

    int Count(){
        return data.size();
    }

    void Replace(T dest, T item){
        data[IndexOf(dest)] = item;
    }

    T At(int index){
        if(index < (int)data.size())return data[index];
        else
        {
            cout << "Out Of Range Exception, At(index)";
            exit(0);
        }
        return data[0];
    }

    T* AtP(int index){
        if(index < (int)data.size())return &data[index];
        else
        {
            cout << "Out Of Range Exception, At(index)";
            exit(0);
        }
        return &data[0];
    }

    bool Contains(T item){
        for (size_t i = 0; i < data.size(); i++){
            if(data[i] == item){
                return true;
            }
        }
        return false;
    }
    
    int IndexOf(T item){
        for (size_t i = 0; i < data.size(); i++){
            if(data[i] == item){
                return i;
            }
        }
        return -1;
    }

    bool Remove(T item){
        int index = IndexOf(item);
        if(index >= 0){
            for (int i = 0; i < (int)data.size()-1; i++){
                if(i >= index){
                    data[i] = data[i+1];
                }
            }
            data.pop_back();
            return true;
        }
        return false;
    }
    
    void RemoveAt(int index){
        if(index >= 0 && index < (int)data.size()){
            for (size_t i = 0; i < data.size(); i++){
                if((int)i >= index){
                    data[i] = data[i+1];
                }
            }
            data.pop_back();
        }
        else 
        {
            cout << "Out Of Range Exception, RemoveAt(index)";
            exit(1);
        }
    }

    T* ToArray(){
        return data.data();
    }

    T Last()
    {
        return data[data.size()-1];
    }

    T* LastP()
    {
        return &data[data.size()-1];
    }

    void SetSize(int size){
        data.resize(size);
    }

    void Clear(){
        data.resize(0);
    }

    void Dispose(){
        Clear();
        free(data);
    }

    void DebugPrint(){
        for (int i = 0; i < data.size(); i++){
            cout << data[i] << "       \n";
        }
    }
    
    T operator[] (int index)
    {
        if(index >= Count()) cout << "Out Of range";

        return At(min(index, Count()-1));
    }
};

struct Color
{
    int r;
    int g;
    int b;
    int stateFlag;

    Color(int _r = 255, int _g = 255, int _b = 255, int state = 0){
        r = _r;
        g = _g;
        b = _b;
        stateFlag = state;
    }

    Color Mix(Color with, float blend = 0.5){
        blend = max((float)0, blend);
        blend = min((float)1, blend);

        return Color((int)(r*(1-blend) + with.r*blend),   (int)(g*(1-blend) + with.g*blend),   (int)(b*(1-blend) + with.b*blend));
    }

    Color Inverse()
    {
        return Color(255-r, 255-g, 255-b, stateFlag);
    }

    Color ChangeState(int newState)
    {
        
        return Color(r, g, b, newState);
    }

    string ANSICode(bool foreground){
        char* result = (char*)malloc(25);
        if(foreground)
            sprintf(result, "\e[38;2;%i;%i;%im", r,g,b);
        else
            sprintf(result, "\e[48;2;%i;%i;%im", r,g,b);

        return string(result);
    }

    bool operator==(const Color& other){
        return (r == other.r && g == other.g && b == other.b && stateFlag == other.stateFlag);
    }
};

class ColorTheme
{
    public:
    Color screenColor;
    Color backgroundColor;
    Color textColor;
    Color highlightColor;
    Color highlightTextColor;

    Color player1;
    Color player2;

    Color currentBackground;
    Color currentForeground;
    int foregroundNestDepth;
    int backgroundNestDepth;

    private:
    List<Color> backgroundHistory;
    List<Color> foregroundHistory;
    

    public:
    ColorTheme(){
        screenColor = Color(20,10,40);
        backgroundColor = Color(99,85,98);
        textColor = Color(224,212,185);
        highlightTextColor = Color(255,255,255, 1);
        highlightColor = Color(217, 216,199, 1);
        player1 = Color(14,201,114, 3);
        player2 = Color(255,102,102, 3);

        currentForeground = screenColor;
        currentForeground = textColor;
        SwapColors(screenColor, textColor);

        foregroundNestDepth = 0;
        backgroundNestDepth = 0;
    };

    ColorTheme(Color _screenColor, Color _backgroundColor, Color _textColor, Color _highlightTextColor, Color _highlightColor, Color _player1, Color _player2){
        screenColor = _screenColor;
        backgroundColor = _backgroundColor;
        textColor = _textColor;
        highlightTextColor = _highlightTextColor;
        highlightColor = _highlightColor;
        player1 = _player1;
        player2 = _player2;
        currentForeground = screenColor;
        currentForeground = textColor;
        SwapColors(screenColor, textColor);

        foregroundNestDepth = 0;
        backgroundNestDepth = 0;
    }

    void Regress(){
        if(backgroundHistory.Count() > 0 && backgroundHistory.Count() > 0){
            currentBackground = backgroundHistory.Last();
            backgroundHistory.RemoveAt(backgroundHistory.Count()-1);
            backgroundNestDepth--;

            currentForeground = foregroundHistory.Last();
            foregroundHistory.RemoveAt(foregroundHistory.Count()-1);
            foregroundNestDepth--;

            SetColors(currentBackground, currentForeground);
        }
    }

    void SwapColors(Color back, Color front){
        backgroundHistory.Add(currentBackground);
        foregroundHistory.Add(currentForeground);

        currentForeground = front;
        currentBackground = back;
        SetColors(currentBackground, currentForeground);
        
        foregroundNestDepth++;
        backgroundNestDepth++;
    }

    void Reset()
    {
        backgroundNestDepth = 0;
        foregroundNestDepth = 0;
        backgroundHistory.Clear();
        foregroundHistory.Clear();
        currentBackground = screenColor;
        currentForeground = textColor;
        SwapColors(screenColor, textColor);
    }

    private:
    void SetColor(Color color, bool foreground = false)
    {
        if(foreground)
            printf("\e[38;2;%i;%i;%im", color.r,color.g,color.b);
        else
            printf("\e[48;2;%i;%i;%im", color.r,color.g,color.b);
    }
    public:
    void SetColors(Color back, Color front)
    {
        printf("\e[38;2;%i;%i;%im", front.r,front.g,front.b);
        printf("\e[48;2;%i;%i;%im", back.r,back.g,back.b);
    }
};

ColorTheme globalTheme;

struct Cell
{
    public:
    string contents;
    Color color;

    Cell(string content = " ", Color _color = Color(0,0,0,0)){
        contents = content;
        color = _color;
    }
};

class Table
{
    int xHold;
    int yHold;

    public:
    int x;
    int y;
    int xOffset;
    int yOffset;
    List<Cell> values;


    const static char vert =  static_cast<char>(186);
    const static char horiz =  static_cast<char>(205);
    const static char cross =  static_cast<char>(206);

    Table(){
        x = 3;
        y = 3;
        xHold = x;
        yHold = y;
        xOffset = 0;
        yOffset = 0;
        values.SetSize(9);
    }

    Table(int xSize, int ySize, int _xOffset, int _yOffset){
        x = xSize;
        y = ySize;
        xOffset = _xOffset;
        yOffset = _yOffset;
        xHold = x;
        yHold = y;
        values.SetSize(x*y);
    }

    void SetCell(string item, int index){
        Cell c = Cell(item, values.At(index).color);
        values.ReplaceAt(index, c);
    }

    void SetCellColor(int index, Color color){
        Cell c = values.At(index);
        c.color = color;
        values.ReplaceAt(index, c);
    }

    void Show(){
        globalTheme.SwapColors(globalTheme.backgroundColor, globalTheme.textColor);
        repeatChar(yOffset, "\n");
        for (int j = 0; j < y; j++){
            globalTheme.SwapColors(globalTheme.screenColor, globalTheme.textColor);
            repeatChar(xOffset, " ");
            globalTheme.Regress();
            for (int i = 0; i < x; i++){
                int index = i + j*x;
                if(index < values.Count()){   
                    if(i > 0 && i < x){
                        cout << vert;
                    }

                    Cell cell = values.At(index);

                    if(cell.contents.empty()) cell.contents = " ";
                    switch(cell.color.stateFlag){
                        case 3:
                            globalTheme.SwapColors(cell.color, globalTheme.highlightTextColor);
                            cout << " " << cell.contents[cell.contents.length()-1] << " ";
                            globalTheme.Regress();
                            break;
                        case 2:
                            cout << " " << cell.contents << " ";
                            break;
                        case 1:
                            globalTheme.SwapColors(cell.color, globalTheme.highlightTextColor);
                            cout << " " << cell.contents[cell.contents.length()-1] << " ";
                            cell.color.stateFlag = 0;
                            SetCellColor(index, cell.color);
                            globalTheme.Regress();
                            break;
                        case 0:
                            //globalTheme.SwapColors(globalTheme.backgroundColor, globalTheme.textColor);
                            cout << " " << values.At(index).contents << " ";
                            cout << globalTheme.textColor.ANSICode(true);
                            //globalTheme.Regress();
                            break;
                    }
                }
            }

            cout << "\n";
            if(j != y-1){
                globalTheme.SwapColors(globalTheme.screenColor, globalTheme.textColor);
                repeatChar(xOffset, " ");
                globalTheme.Regress();
                for (int i = 0; i < (x*4)-1; i++){
                    if(i > 0 && (i+1) % 4 == 0) cout << cross;
                    else cout << horiz;
                }
            }
            cout << "\n";
        }
        globalTheme.Regress();
    }
    
};

class ConsoleManager
{
    public:
    HWND console;
    RECT ConsoleRect;
    RECT ConsoleRectHold;
    float fontSize;
    COORD fontAspect;
    int *sizeChar;
    int *pixelSize;
    bool cursorHidden;
    HANDLE consoleHandle;

    ConsoleManager(){
        system("cls");
        console = GetConsoleWindow();
        GetWindowRect(console, &ConsoleRect);
        GetWindowRect(console, &ConsoleRectHold);
        fontAspect.X = 12;
        fontAspect.Y = 16;
        fontSize = 1;
        sizeChar = GetConsoleSizeChar();
        SetSize(20,30);
        cursorHidden = false;
        consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    int* GetConsoleSizeChar(){
        CONSOLE_SCREEN_BUFFER_INFO info;
        int *result = (int*)malloc(sizeof(int) * 2);

        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
        result[0] = info.srWindow.Right - info.srWindow.Left;
        result[1] = info.srWindow.Bottom - info.srWindow.Top;
        sizeChar = result;

        return result;
    }

    void FitToTable(Table fitTo, int ySizeBelow){
        int x = fitTo.xOffset*2 + 4*fitTo.x -1;
        int y = ySizeBelow + fitTo.yOffset + 2*fitTo.y-1;
        
        SetSize(x,y, fitTo.xOffset, fitTo.yOffset);
        if(cursorHidden)  HideCursor();
    }

    void SetSize(int charX, int charY, int Xpx = 0, int Ypx = 0){
        CONSOLE_SCREEN_BUFFER_INFOEX bufferInfo;
        bufferInfo.cbSize = sizeof(bufferInfo);
        GetConsoleScreenBufferInfoEx(consoleHandle, &bufferInfo);

        COORD c;
        c.X = charX;
        c.Y = charY;

        bufferInfo.dwSize = c;
        bufferInfo.dwMaximumWindowSize = c;
        bufferInfo.srWindow.Left = 0;
        bufferInfo.srWindow.Right = charX;
        bufferInfo.srWindow.Top = 0;
        bufferInfo.srWindow.Bottom = charY;

        SetConsoleScreenBufferInfoEx(consoleHandle, &bufferInfo);
        SetConsoleScreenBufferSize(consoleHandle, c);

        ShowScrollBar(GetConsoleWindow(), SB_VERT, 0);

        GetWindowRect(console, &ConsoleRect);
        GetWindowRect(console, &ConsoleRectHold);
        sizeChar = GetConsoleSizeChar();


        if(cursorHidden)  HideCursor();
    }

    BOOL SetFontSize(float size){
        fontSize = size;

        HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_FONT_INFOEX info;
        info.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        if (!GetCurrentConsoleFontEx(output, false, &info))
            return false;
        info.dwFontSize.X = (int)(fontAspect.X * size);
        info.dwFontSize.Y = (int)(fontAspect.Y * size);
        WINBOOL result = SetCurrentConsoleFontEx(output, false, &info);
        GetWindowRect(console, &ConsoleRectHold);

        if(cursorHidden)  HideCursor();

        return result;
    }

    bool RectChanged(){
        GetWindowRect(console, &ConsoleRect);
        if (ConsoleRect.bottom-ConsoleRect.top != ConsoleRectHold.bottom-ConsoleRectHold.top || ConsoleRect.right-ConsoleRect.left != ConsoleRectHold.right-ConsoleRectHold.left){
            GetWindowRect(console, &ConsoleRectHold);
            sizeChar = GetConsoleSizeChar();
            if(cursorHidden)  HideCursor();
            return true;
        }
        else return false;
    }
    
    void HideCursor(){
        cursorHidden = true;
        cout << "\e[?25l";
    }

    void ShowCursor(){
        cursorHidden = false;
        cout << "\e[?25h";
    }

    void SetScreenColor(Color color){
        CursorHome();
        globalTheme.SwapColors(color, globalTheme.currentForeground);
        system("cls");
        if(cursorHidden)  HideCursor();
        globalTheme.screenColor = color;
        globalTheme.Reset();
    }
    
    void CursorHome()
    {
        HANDLE console;
        COORD xy = {0,0};
        console = GetStdHandle( STD_OUTPUT_HANDLE );
        SetConsoleCursorPosition( console, xy );
        if(cursorHidden)  HideCursor();
    }

    void PrintCenter(bool newline, const char* fmt, ...)
    {
        GetConsoleSizeChar();
        if(cursorHidden)  HideCursor();

        va_list arg;
        va_start(arg, fmt);
        char finalString[1024];
        vsprintf(finalString, fmt, arg);
        int first = (sizeChar[0]-strlen(finalString))/2;
        int last = (sizeChar[0]-strlen(finalString)) - first;
        first += sizeChar[0] - (first + last + strlen(finalString));

        repeatChar(first, " ");
        cout << finalString;
        repeatChar(last, " ");

        if(newline)
            cout <<"\n";
        va_end(arg);
    }
};

ConsoleManager console;

struct PlayerData
{
    public:
    string symbol;
    string label;
    Color color;
    

    PlayerData(){}

    PlayerData(string _symbol, string _label, Color _color){
        symbol = _symbol;
        label = _label;
        color = _color;
    }

    bool operator==(PlayerData& other){
        return (color == other.color && label == other.label && symbol == other.symbol);
    }
};

class TicTacToeBoard
{
    public:
    int dim;
    int size;

    List<int> positions1;
    List<int> positions2;
    PlayerData player1;
    PlayerData player2;
    int playerTurn;
    
    int selectedPosition;
    Table displayTable;

    int winLength;
    bool gameOver;
    string winningLabel;

    public: 
    TicTacToeBoard(){}
    TicTacToeBoard(int _size, int lengthToWin, PlayerData _player1, PlayerData _player2, int xOffset = 4, int yOffset = 1){
        dim = 2;
        size = _size;
        selectedPosition = rand() % (size*size);
        displayTable = Table(size, size, xOffset, yOffset);
        gameOver = false;
        winLength = lengthToWin;
        player1 = _player1;
        player2 = _player2;
        winningLabel = "Nobody";
        playerTurn = 0;
    }

    void MoveSelected(string dir){
        if(dir == "up" && selectedPosition > size-1) selectedPosition -= size;
        if (dir == "down" && selectedPosition <= (size*size)-size-1) selectedPosition += size;
        if (dir == "left" && selectedPosition > 0 && (selectedPosition) % size != 0) selectedPosition--;
        if (dir == "right" && selectedPosition < size*size && (selectedPosition+1) % size != 0) selectedPosition++;
    }

    void SetAtIndex(int index, PlayerData player, bool redraw = false){
        if(selectedPosition != index){
            MoveCursorTo(index, 100);
        }
        if(player == player1) positions1.Add(index);
        if(player == player2) positions2.Add(index);

        if(positions1.Count() + positions2.Count() >= size*size) gameOver = true;

        if(redraw){
            console.CursorHome();
            Show();
            cout << "\e[s";
        }
    }

    void MoveCursorTo(int index, int sleep){
        int *currentPos = index2D(selectedPosition);
        int *targetPos = index2D(index);
        int xBias = targetPos[1] - currentPos[1];
        int yBias = targetPos[0] - currentPos[0];

        int totalSteps = abs(xBias) + abs(yBias);
        for(int i = 0; i <  totalSteps; i++){
            
            if(xBias < 0){
                MoveSelected("left");
                xBias++;
            }
            if(xBias > 0) 
            {
                MoveSelected("right");
                xBias--;
            }

            srand(time(0)+i);
            if(sleep > 0 && xBias != 0){
                Sleep(rand() % sleep + sleep/2);
                Show();
            }
            

            if(yBias < 0){
                MoveSelected("up");
                yBias++;
            }
            if(yBias > 0) 
            {
                MoveSelected("down");
                yBias--;
            }

            srand(time(0)*i);
            if(sleep > 0 && yBias != 0){
                Sleep(rand() % sleep + sleep/2);
                Show();
            }
        }
    }

    int* index2D(int index){
        int *indexes = (int*)malloc(sizeof(int)*2);
        indexes[0] = index / size;
        indexes[1] = index % size;
        return indexes;
    }

    List<int> GetNeighborIndicies(int index){
        List<int> indicies;
        indicies.SetSize(9);

        int pos = 0;
        for (int i = -1; i <= 1; i++){
            for (int j = -1; j <= 1; j++){
                int value = index + (i*size + j);
                int rowV = value/size;
                int colV = value %size;
                int rowI = index/size;
                int colI = index%size;
                if(abs(rowI-rowV) > 1 || abs(colI - colV) > 1 || value > size*size || value < 0){
                    indicies.ReplaceAt(pos, index);
                }
                else
                {
                    indicies.ReplaceAt(pos, value);
                }
                pos++;
            }
        }

        return indicies;
    }

    List<int> FindNeighborMatches(int index){
        List<int> matches;

        int player = -1;
        if(positions1.Contains(index)) player = 0;
        else if(positions2.Contains(index)) player = 1;
        else return List<int>();

        int *indicies = GetNeighborIndicies(index).ToArray();
        
        for (int i = 0; i < 9; i++){
            int checkIndex = indicies[i];
            if(indicies[i] != index){
                if(player == 0){
                    if(positions1.Contains(checkIndex)){
                        matches.Add(checkIndex);
                    }
                }
                if(player == 1){
                    if(positions2.Contains(checkIndex)){
                        matches.Add(checkIndex);
                    }
                }
            }
        }

        return matches;
    }

    //iterates through a string of matches and returns the length
    int MatchIterator(int index, Color highlight, string theoreticalPiece = "", int startIndex = -1, bool second = false, int depth = 1, int dir = -1){
        if(second && dir != -1){
            displayTable.SetCellColor(index, highlight);
        }
        
        int depthReturn = depth;
        bool beggining = depth == 1 && dir == -1 && !second;
        bool stringEnd = true;

        if(beggining){
            if(theoreticalPiece == player1.symbol && !positions1.Contains(index)){
                positions1.Add(index);
                startIndex = index;
            }
            if(theoreticalPiece == player2.symbol  && !positions2.Contains(index)){
                positions2.Add(index);
                startIndex = index;
            }
        }


        List<int> matches = FindNeighborMatches(index);
        List<int> indicies = GetNeighborIndicies(index);
        if(beggining){
            for (int i = 0; i < matches.Count(); i++){
                List<int> nextMatches = FindNeighborMatches(matches.At(i));

                for (int j = 0; j < nextMatches.Count(); j++){
                    int neighborDirection = GetNeighborIndicies(matches.At(i)).IndexOf(nextMatches.At(j));
                    int thisDirection = indicies.IndexOf(matches.At(i));
                    //cout << "N: " << neighbor << " T: " << displayTable << " Depth: " << depth << "\n";
                    if(neighborDirection == thisDirection){
                        depthReturn = max(MatchIterator(matches.At(i), highlight, theoreticalPiece, startIndex, second, depth+1, thisDirection), depthReturn);
                        stringEnd = false;
                    }
                }
            }

            //start over if no specific direction found
            if(matches.Count() > 0 && stringEnd){
                depthReturn = max(MatchIterator(index, highlight, theoreticalPiece, startIndex, false, 1, indicies.IndexOf(matches.At(0))), depthReturn);
                stringEnd = false;
            }
        }

        if(!beggining){
            for (int i = 0; i < matches.Count(); i++){
                int last = dir;
                int next = indicies.IndexOf(matches.At(i));
                //cout << "N: " << last << " T: " << next << " Depth: " << depth << "\n";
                if(last == next){
                    depthReturn = max(MatchIterator(matches.At(i), highlight,theoreticalPiece,startIndex, second, depth+1, last), depthReturn);
                    stringEnd = false;
                }
            }
        }

        if(!second && stringEnd){
            depthReturn = max(MatchIterator(index, highlight, theoreticalPiece, startIndex, true, 1, 8-dir), depthReturn);
        }

        if(second && stringEnd){
            if(theoreticalPiece == player1.symbol)
                positions1.Remove(startIndex);
            if(theoreticalPiece == player2.symbol)
                positions2.Remove(startIndex);
            depthReturn = depth;
        }

        return depthReturn;
    }
     
    bool FindWins(int length){
        for (int i = 0; i < size * size; i++){
            int match = MatchIterator(i, Color(0,0,0,0));
            if(match >= length){
                if(positions1.Contains(i)){
                    MatchIterator(i, player1.color);
                    winningLabel = player1.label;
                }
                else 
                {
                    MatchIterator(i, player2.color);
                    winningLabel = player2.label;
                }

                gameOver = true;
                return true;
            }
        }
        return false;
    }

    void Show(bool showCursor = true){
        for (int i = 0; i < size*size; i++){
            
            if(positions1.Contains(i))
                displayTable.SetCell(player1.symbol, i);
            else if(positions2.Contains(i))
                displayTable.SetCell(player2.symbol, i);
            else displayTable.SetCell(" ", i);
        }
        if(displayTable.values.At(selectedPosition).color.stateFlag <= 1 && showCursor){
            if(playerTurn == 1)
               displayTable.SetCellColor(selectedPosition, player1.color.Mix(Color(0,0,0), 0.2).ChangeState(1));
            else if(playerTurn == 2)
               displayTable.SetCellColor(selectedPosition, player2.color.Mix(Color(0,0,0), 0.2).ChangeState(1));
            else displayTable.SetCellColor(selectedPosition, globalTheme.highlightColor);
        }

        console.CursorHome();
        displayTable.Show();

    }

};

class Player
{
    public:
    PlayerData data;
    bool bot;

    Player(){
        data = PlayerData("O", "Player", Color(200,200,200));
        bot = false;
    }

    Player(string symbol, string label, Color color, bool isBot = false){
        data = PlayerData(color.ANSICode(true) + symbol, label, color);
        bot = isBot;
    }

    int ChooseMove(TicTacToeBoard *board){
        if(bot){
            PlayerData opponent;
            if(board->player1 == data) opponent = board->player2;
            else opponent = board->player1;
            float strength = 0;
            int bestIndex = rand() % 9;
            for (int i = 0; i < board->size * board->size; i++){
                if(board->positions1.Contains(i) || board->positions2.Contains(i)) continue;
                float opponentMatch = board->MatchIterator(i, Color(0,0,0,0), opponent.symbol);
                float selfMatch = board->MatchIterator(i, Color(0,0,0,0), data.symbol);

                float thisStrength = opponentMatch + selfMatch;
                if(opponentMatch >= board->winLength) thisStrength *= 10;
                if(selfMatch >= board->winLength) thisStrength *= 20;

                srand(time(0)+i);
                thisStrength += (rand() % 3)/2;

                srand(time(0)*i);
                thisStrength -= (rand() % 3)/2;

                if(thisStrength > strength){
                    strength = thisStrength;
                    bestIndex = i;
                }
            }
    
            return bestIndex;
        }
        else
        {
            while(true){
                string in = GetKeyboardInput();
                if(in == "\r" && !board->positions1.Contains(board->selectedPosition) && !board->positions2.Contains(board->selectedPosition))
                    return board->selectedPosition;
                else board->MoveSelected(in);
                board->Show();
            }
        }
    }
};

struct Option
{
    public:
    string name;
    float* variable;
    List<string> labels;
    float min;
    float max;
    bool requireRefresh;
    string specialKey;
    float stepSize;
    int optionSelected;

    Option()
    {
        name = "NULL";
        labels = List<string>();
        min = 0;
        max = 1;
    }    

    Option(string _name, float* _variable, bool _requireRefresh, float _min, float _max, string _specialKey = "", float _stepSize = 1)
    {
        name = _name;
        variable = _variable;
        labels = List<string>();
        min = _min;
        max = _max;
        requireRefresh = _requireRefresh;
        specialKey = _specialKey;
        stepSize = _stepSize;
        optionSelected = 0;
    }

};

class Menu
{
    public:
    List<Option> items;
    Option *selected;
    int selectedOption;

    Menu()
    {
        selectedOption = -1;
    }

    void Show()
    {
        for (int i = 0; i < items.Count(); i++)
        {
            console.PrintCenter(true, items[i].name.data());
            Color highlight = (i == selectedOption) ? globalTheme.highlightColor.Mix(globalTheme.screenColor,0.8) : globalTheme.currentBackground;
            Color text = (i == selectedOption) ? globalTheme.highlightTextColor : globalTheme.textColor;

            globalTheme.SwapColors(highlight, text);
            if(items[i].labels.Count() > 0 && items[i].optionSelected < items[i].labels.Count()){
                console.PrintCenter(true, "%c %s %c", 17, items[i].labels[items[i].optionSelected].data(), 16);
            }
            else console.PrintCenter(true, "%c %.2f %c", 17, *items[i].variable, 16);
            globalTheme.Regress();
            cout << "\n";

        }
    }

    Option* acceptArrowInput()
    {
        string input = GetKeyboardInput();
        if(input == "up" && selectedOption > 0){
            selectedOption--;
        }
        if(input == "down" && selectedOption < items.Count()-1){
            selectedOption++;
        }
        if(input == "left"){
            
            if(*selected->variable >= selected->min + selected->stepSize){
                 *selected->variable -= selected->stepSize;
                 selected->optionSelected--;
                 return selected;
            }
        }
        if(input == "right"){
            if(*selected->variable <= selected->max - selected->stepSize){
                *selected->variable += selected->stepSize;
                selected->optionSelected++;
                return selected;
            }
        }

        for (int i = 0; i < items.Count(); i++)
        {
            Option *item = items.AtP(i);
            if(input == item->specialKey)
            {
                if(*item->variable <= item->max - item->stepSize){
                    *item->variable += item->stepSize;
                    selected = item;
                    selectedOption = i;
                    return item;
                }
            }
        }

        selected = items.AtP(selectedOption);

        if(input == "esc"){
            globalTheme.SwapColors(globalTheme.screenColor, globalTheme.currentForeground);
            console.SetSize(console.sizeChar[0], console.sizeChar[1] + 3);
            cout << "\n";
            repeatChar(console.sizeChar[0], "-");
            cout << "\n\nAgain to close...";
            globalTheme.SwapColors(globalTheme.screenColor, globalTheme.currentForeground);
            exit(0);
        }
        return NULL;
    }

    void Add(string name, float* variable, bool requireRefresh, string specialKey, float stepSize, float minValue, float maxValue, int optionCount, ...)
    {
        
        va_list labelArguments;
        va_start(labelArguments, optionCount);
        items.Add(Option(name, variable, requireRefresh, minValue, maxValue, specialKey, stepSize));
        if(optionCount > 0){
            items.LastP()->labels.Add(string(va_arg(labelArguments, const char*)));

            for (int i = 0; i < optionCount-1; i++)
            {
                items.LastP()->labels.Add(string(va_arg(labelArguments, const char*)));
            }
        }
        va_end(labelArguments);
       
        selectedOption = 0;
        selected = items.AtP(selectedOption);
    }
};


int main(){
    //Begin init ------------------
    float boardSize = 3;
    float winLength = 3;
    float playerMode = 0;
    float themeNum = 0;
    float fontSize = 1.8;
    float play = 0;

    //init Main menu
    Menu settings;
    settings.Add("Game Mode", &playerMode, false, "", 1, 0, 2, 3, "Human vs. Human", "Human vs. Bot", "Bot vs. Bot");
    settings.Add("Board Size", &boardSize, true, "", 1, 3, 6, 4, "3 x 3", "4 x 4", "5 x 5", "6 x 6");
    settings.Add("Color Theme", &themeNum, true, "", 1, 0, 4, 5, "Sunset", "Purple", "Dark", "Candy", "Random");
    settings.Add("Font Size", &fontSize, false, "", 0.1, 0.99, 2.11, 0);
    settings.Add("", &play, false, "\r", 1, 0, 1, 2, "Enter to Play", "Loading");
    

    startofapp:
    system("cls");
    //init color themes 
    srand(time(0));
    //                    screenColor        backgroundColor      textColor             highlightTextColor     highlightColor           player1Color             player2Color
    List<ColorTheme> themes;
    themes.Add(ColorTheme(Color(49, 29, 63), Color(82, 37, 70),   Color(252, 218, 183), Color(255,255,255, 1), Color(226, 62, 87, 1),   Color(68, 235, 163, 3),  Color(235, 68, 110, 3))); //sunset
    themes.Add(ColorTheme());
    themes.Add(ColorTheme(Color(21, 21, 21), Color(2, 89, 85),    Color(200,200,200),   Color(255,255,255, 1), Color(0, 145, 124, 1),   Color(68, 235, 163, 3),  Color(235, 68, 110, 3))); //dark
    themes.Add(ColorTheme(Color(60, 60, 80), Color(48, 118, 114), Color(255, 221, 204), Color(255,255,255, 1), Color(255, 153, 153, 1), Color(166, 230, 174, 3), Color(224, 159, 193, 3))); //candy
    themes.Add(ColorTheme(Color((rand()*1)%255, (rand()*1)%255, (rand()*1)%255), Color((rand()*2)%255, (rand()*2)%255, (rand()*2)%255), Color((rand()*3)%255, (rand()*3)%255, (rand()*3)%255), 
               Color((rand()*4)%255,(rand()*4)%255,(rand()*4)%255, 1), Color((rand()*4)%255,(rand()*4)%255,(rand()*4)%255, 1), Color((rand()*4)%255,(rand()*4)%255,(rand()*4)%255, 3), Color((rand()*4)%255,(rand()*4)%255,(rand()*4)%255, 3))); //candy

    //we are not playing yet
    play = 0;
    settings.items.LastP()->optionSelected = play;
    
    //init game objects
    Player player1;
    Player player2;

    //initiate console
    TicTacToeBoard board = TicTacToeBoard(boardSize, winLength, player1.data, player2.data, 10, 2);
    console.FitToTable(board.displayTable, 18);
    globalTheme = themes[themeNum];
    console.SetScreenColor(globalTheme.screenColor);
    console.SetFontSize(fontSize);
    console.HideCursor();

    //initial show
    board.Show();
    cout << "\e[s";
    settings.Show();
    globalTheme.SwapColors(globalTheme.textColor, globalTheme.highlightColor);
    console.PrintCenter(true, "Use the arrow keys");
    globalTheme.Regress();
    console.CursorHome();

    //show menu and run menu logic
    while(play == 0){
        Option *update = settings.acceptArrowInput();
        if(update != NULL){
            if(update->requireRefresh){
                board = TicTacToeBoard(boardSize, winLength, player1.data, player2.data, 10, 2);
                console.CursorHome();
                console.FitToTable(board.displayTable, 18);
                globalTheme = themes[themeNum];
                console.SetScreenColor(globalTheme.screenColor);

                board.Show();
                cout << "\e[s";
            }
        }

        if(play == 0){
            console.SetFontSize(fontSize);
            cout << "\e[u";
            settings.Show();

        }
    }

    //set up game based on final menu choices
    if(playerMode == 0){ // human vs. human
        player1 = Player("X", "Player 1", globalTheme.player1);
        player2 = Player("O", "Player 2", globalTheme.player2);
    }
    if(playerMode == 1){ // human vs. bot
        player1 = Player("X", "You", globalTheme.player1);
        player2 = Player("O", "The Destroyer", globalTheme.player2, true);
    }
    if(playerMode == 2){ // bot vs. bot
        player1 = Player("X", "Mr. Tic Tac", globalTheme.player1, true);
        player2 = Player("O", "The Destroyer", globalTheme.player2, true);
    }
    if(boardSize > 4) winLength = boardSize-1;
    else winLength = boardSize;
    board = TicTacToeBoard(boardSize, winLength, player1.data, player2.data, 10, 2);   
    
    //end init ---------------------------------------------------------------------------
    startofrun:
    system("cls");
    console.SetScreenColor(globalTheme.screenColor);
    int moveFirst = rand() % 2; //who goes first
    //start game loop
    while(true){
        console.CursorHome();
        board.Show();
        cout << "\e[s";
        
        if(board.gameOver){
            cout << "\e[u";
            cout << "\e[0J";
            
            if(board.winningLabel == player1.data.label){
                globalTheme.SwapColors(player1.data.color.Mix(Color(0,0,0)), globalTheme.highlightTextColor);
            }
            else if(board.winningLabel == player2.data.label){
                globalTheme.SwapColors(player2.data.color.Mix(Color(0,0,0)), globalTheme.highlightTextColor);
            }
            else globalTheme.SwapColors(Color(124,124,124), globalTheme.textColor);
            
            console.PrintCenter(true, " %s Won ", board.winningLabel.data());
            
            cout << "\n";


            if(board.winningLabel == player1.data.label) globalTheme.SwapColors(player1.data.color.Mix(globalTheme.screenColor, 0.8), globalTheme.currentForeground);
            else if(board.winningLabel == player2.data.label) globalTheme.SwapColors(player2.data.color.Mix(globalTheme.screenColor, 0.8), globalTheme.currentForeground);
            else globalTheme.SwapColors(Color(124,124,124).Mix(globalTheme.screenColor, 0.8), globalTheme.currentForeground);

            console.PrintCenter(true,"%c%c%c Play Again", 60, 196, 217);
            cout << "\n";
            console.PrintCenter(true, "ESC to Settings");
            globalTheme.Regress();
            cout << "\n";
            
            

            string input;
            while(input != "esc" && input != "\r"){
                input = GetKeyboardInput();
                if(input == "esc"){
                    system("cls");
                    goto startofapp;
                }
                if(input == "\r"){
                    board = TicTacToeBoard(boardSize, winLength, player1.data, player2.data, 10, 2);
                    goto startofrun;
                }
            }
        }

        
        if(abs(moveFirst) == 1){
            if(!board.gameOver){ //User takes a move
                cout << "\e[0u";
                if(player1.data.label == "You") console.PrintCenter(true,"Your Turn!", player1.data.label.data());
                else console.PrintCenter(true,"%s's Turn!", player1.data.label.data());
                cout << "\e[0J" << "\n";
                if(!player1.bot){
                    console.PrintCenter(true, "Arrow Keys Move");
                    console.PrintCenter(true, "%c%c%c To Place",60, 196,217);
                }
                board.playerTurn = 1;
                board.SetAtIndex(player1.ChooseMove(&board), player1.data, true);
            }
        }

        if(!board.FindWins(winLength) && !board.gameOver) //bot takes a move - and moves the cursor
        { 
            cout << "\e[0u";
            console.PrintCenter(true,"%s's Turn!", player2.data.label.data());
            cout << "\e[0J" << "\n";
            if(!player2.bot){
                console.PrintCenter(true, "Arrow Keys Move");
                console.PrintCenter(true, "%c%c%c To Place",60, 196,217);
            }
            board.playerTurn = 2;
            board.SetAtIndex(player2.ChooseMove(&board), player2.data);
        }
        board.FindWins(winLength);

        if(console.RectChanged()) console.FitToTable(board.displayTable, 18);
        moveFirst = -1;
    }
    return 0;
}