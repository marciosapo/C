#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/fl_ask.H>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <chrono>
#include <thread>

namespace vars {
    std::string turno;
    std::string player1;
    std::string cpu;
    Fl_Button* board[9];
    bool isRunning = false;
    bool showing = false;
    bool turnoChanging = false;
    Fl_Box* boxes[13];
    int playerVictorys = 0;
    int cpuVictorys = 0;
    int draws = 0;
}

std::string pl1, cp, sturno, cv, pv, dv;

struct ButtonData {
    int id;
    class MainWindow* mainWin;
};



class MainWindow : public Fl_Window {
    public:
        MainWindow(int width, int height, const char* title);
        void MakeGame();
        void ReMake();
        void fillBoard();
        void cpuTurno(Fl_Widget* widget, void* data);
        void changeTurno(Fl_Widget* widget, void* data);
        void EndGame();
    private:
        static void buttonClick(Fl_Widget* widget, void* data);
        static void buttonStartClick(Fl_Widget* widget, void* data);
        static bool checkResult(Fl_Widget* widget, void* data);
        static bool checkResultCPU(Fl_Widget* widget, int id);
        std::vector<int*> buttonIds; // To store dynamically allocated IDs
};

void show_info_message(Fl_Widget* widget) {
    if(vars::showing){
        std::string message = "Agora é o turno do " + vars::turno;
        fl_message_icon()->hide(); // Set the icon to the info icon (default is "i" in a circle)
        fl_message("%s", message.c_str());
        vars::showing = false;
    }
}

void show_winner_message(std::string whoWin) {
    if(vars::showing){
        std::string message = "Vencedor: " + whoWin;
        fl_message_icon()->hide(); // Set the icon to the info icon (default is "i" in a circle)
        fl_message("%s", message.c_str());
        vars::showing = false;
    }
}

void show_draw_message() {
    if(vars::showing){
        std::string message = "Jogo terminou em empate";
        fl_message_icon()->hide(); // Set the icon to the info icon (default is "i" in a circle)
        fl_message("%s", message.c_str());
        vars::showing = false;
    }
}

void MainWindow::changeTurno(Fl_Widget* widget, void* data){
    if(vars::isRunning == false) { return; }
    if(vars::showing == true) { return; }
    MainWindow* mainWin = static_cast<MainWindow*>(data);
    if(vars::turno=="Player 1"){ vars::turno = "CPU"; }
    else { vars::turno = "Player 1"; }
    vars::turnoChanging = false;
    vars::showing = true;
    show_info_message(mainWin);
    mainWin->ReMake();
    if(vars::turno=="CPU") { mainWin->cpuTurno(widget, data); }
}

void MainWindow::EndGame() {
    if(vars::isRunning == true) { return; }
    pv = "PLAYER VICTORIES: " + std::to_string(vars::playerVictorys);
    cv = "CPU VICTORIES: " + std::to_string(vars::cpuVictorys);
    dv = "EMPATES: " + std::to_string(vars::draws);
    vars::boxes[6]->label("");
    vars::boxes[7]->label("");
    vars::boxes[8]->label("");
    vars::boxes[10]->label(pv.c_str());
    vars::boxes[11]->label(cv.c_str());
    vars::boxes[12]->label(dv.c_str());
    for(int i = 0; i < 9; i++){
        vars::board[i]->label("");
    }
}

bool MainWindow::checkResult(Fl_Widget* widget, void* data){
    MainWindow* mainWin = static_cast<MainWindow*>(data);
    int ended = 0;
    bool winner = false;
    bool empate = false;
    std::string whoWin = "";
    ButtonData* buttonData = static_cast<ButtonData*>(data); // Retrieve the ButtonData structure
    int id = buttonData->id;
    if(id==0){
        if(vars::board[0]->label() == vars::board[1]->label() && vars::board[0]->label() == vars::board[2]->label()) {
            winner = true;
            if(vars::board[0]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[0]->label() == vars::board[3]->label() && vars::board[0]->label() == vars::board[6]->label()) {
            winner = true;
            if(vars::board[0]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[0]->label() == vars::board[4]->label() && vars::board[0]->label() == vars::board[8]->label()) {
            winner = true;
            if(vars::board[0]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==1){
        if(vars::board[1]->label() == vars::board[0]->label() && vars::board[1]->label() == vars::board[2]->label()) {
            winner = true;
            if(vars::board[1]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[1]->label() == vars::board[4]->label() && vars::board[1]->label() == vars::board[7]->label()) {
            winner = true;
            if(vars::board[1]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==2){
        if(vars::board[2]->label() == vars::board[0]->label() && vars::board[2]->label() == vars::board[1]->label()) {
            winner = true;
            if(vars::board[2]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[2]->label() == vars::board[4]->label() && vars::board[2]->label() == vars::board[6]->label()) {
            winner = true;
            if(vars::board[2]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[2]->label() == vars::board[5]->label() && vars::board[2]->label() == vars::board[8]->label()) {
            winner = true;
            if(vars::board[2]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==3){
        if(vars::board[3]->label() == vars::board[0]->label() && vars::board[3]->label() == vars::board[6]->label()) {
            winner = true;
            if(vars::board[3]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[3]->label() == vars::board[4]->label() && vars::board[2]->label() == vars::board[5]->label()) {
            winner = true;
            if(vars::board[3]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==4){
        if(vars::board[4]->label() == vars::board[0]->label() && vars::board[4]->label() == vars::board[5]->label()) {
            winner = true;
            if(vars::board[4]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[4]->label() == vars::board[2]->label() && vars::board[4]->label() == vars::board[6]->label()) {
            winner = true;
            if(vars::board[4]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[4]->label() == vars::board[3]->label() && vars::board[4]->label() == vars::board[5]->label()) {
            winner = true;
            if(vars::board[4]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==5){
        if(vars::board[5]->label() == vars::board[2]->label() && vars::board[5]->label() == vars::board[8]->label()) {
            winner = true;
            if(vars::board[5]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[5]->label() == vars::board[4]->label() && vars::board[4]->label() == vars::board[3]->label()) {
            winner = true;
            if(vars::board[5]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==6){
        if(vars::board[6]->label() == vars::board[7]->label() && vars::board[6]->label() == vars::board[8]->label()) {
            winner = true;
            if(vars::board[6]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[6]->label() == vars::board[3]->label() && vars::board[6]->label() == vars::board[0]->label()) {
            winner = true;
            if(vars::board[6]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[6]->label() == vars::board[4]->label() && vars::board[6]->label() == vars::board[2]->label()) {
            winner = true;
            if(vars::board[6]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==7){
        if(vars::board[7]->label() == vars::board[6]->label() && vars::board[7]->label() == vars::board[8]->label()) {
            winner = true;
            if(vars::board[7]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[7]->label() == vars::board[4]->label() && vars::board[7]->label() == vars::board[1]->label()) {
            winner = true;
            if(vars::board[7]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==8){
        if(vars::board[8]->label() == vars::board[6]->label() && vars::board[8]->label() == vars::board[7]->label()) {
            winner = true;
            if(vars::board[8]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[8]->label() == vars::board[0]->label() && vars::board[8]->label() == vars::board[4]->label()) {
            winner = true;
            if(vars::board[8]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(winner == false){
        int contador = 0;
        for(int i = 0; i < 9; i++){
            if(vars::board[i]->label() == vars::player1 || vars::board[i]->label() == vars::cpu) { contador++; }
        }
        if(contador==9){ empate = true; }
    }
    if(winner == true) {
        vars::isRunning = false;
        ended = true;
        vars::showing = true;
        if(whoWin == "Player 1") { vars::playerVictorys++; }
        else { vars::cpuVictorys++; }
        mainWin->EndGame();
        show_winner_message(whoWin);
    }
    if(empate == true) {
        vars::isRunning = false;
        ended = true;
        vars::showing = true;
        vars::draws++;
        mainWin->EndGame();
        show_draw_message();
    }
    return ended;
}

bool MainWindow::checkResultCPU(Fl_Widget* widget, int id){
    int ended = 0;
    bool winner = false;
    bool empate = false;
    std::string whoWin = "";
    MainWindow* mainWin = static_cast<MainWindow*>(widget->user_data());
    if(id==0){
        if(vars::board[0]->label() == vars::board[1]->label() && vars::board[0]->label() == vars::board[2]->label()) {
            winner = true;
            if(vars::board[0]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[0]->label() == vars::board[3]->label() && vars::board[0]->label() == vars::board[6]->label()) {
            winner = true;
            if(vars::board[0]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[0]->label() == vars::board[4]->label() && vars::board[0]->label() == vars::board[8]->label()) {
            winner = true;
            if(vars::board[0]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==1){
        if(vars::board[1]->label() == vars::board[0]->label() && vars::board[1]->label() == vars::board[2]->label()) {
            winner = true;
            if(vars::board[1]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[1]->label() == vars::board[4]->label() && vars::board[1]->label() == vars::board[7]->label()) {
            winner = true;
            if(vars::board[1]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==2){
        if(vars::board[2]->label() == vars::board[0]->label() && vars::board[2]->label() == vars::board[1]->label()) {
            winner = true;
            if(vars::board[2]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[2]->label() == vars::board[4]->label() && vars::board[2]->label() == vars::board[6]->label()) {
            winner = true;
            if(vars::board[2]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[2]->label() == vars::board[5]->label() && vars::board[2]->label() == vars::board[8]->label()) {
            winner = true;
            if(vars::board[2]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==3){
        if(vars::board[3]->label() == vars::board[0]->label() && vars::board[3]->label() == vars::board[6]->label()) {
            winner = true;
            if(vars::board[3]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[3]->label() == vars::board[4]->label() && vars::board[2]->label() == vars::board[5]->label()) {
            winner = true;
            if(vars::board[3]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==4){
        if(vars::board[4]->label() == vars::board[0]->label() && vars::board[4]->label() == vars::board[5]->label()) {
            winner = true;
            if(vars::board[4]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[4]->label() == vars::board[2]->label() && vars::board[4]->label() == vars::board[6]->label()) {
            winner = true;
            if(vars::board[4]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[4]->label() == vars::board[3]->label() && vars::board[4]->label() == vars::board[5]->label()) {
            winner = true;
            if(vars::board[4]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==5){
        if(vars::board[5]->label() == vars::board[2]->label() && vars::board[5]->label() == vars::board[8]->label()) {
            winner = true;
            if(vars::board[5]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[5]->label() == vars::board[4]->label() && vars::board[4]->label() == vars::board[3]->label()) {
            winner = true;
            if(vars::board[5]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==6){
        if(vars::board[6]->label() == vars::board[7]->label() && vars::board[6]->label() == vars::board[8]->label()) {
            winner = true;
            if(vars::board[6]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[6]->label() == vars::board[3]->label() && vars::board[6]->label() == vars::board[0]->label()) {
            winner = true;
            if(vars::board[6]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[6]->label() == vars::board[4]->label() && vars::board[6]->label() == vars::board[2]->label()) {
            winner = true;
            if(vars::board[6]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==7){
        if(vars::board[7]->label() == vars::board[6]->label() && vars::board[7]->label() == vars::board[8]->label()) {
            winner = true;
            if(vars::board[7]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[7]->label() == vars::board[4]->label() && vars::board[7]->label() == vars::board[1]->label()) {
            winner = true;
            if(vars::board[7]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(id==8){
        if(vars::board[8]->label() == vars::board[6]->label() && vars::board[8]->label() == vars::board[7]->label()) {
            winner = true;
            if(vars::board[8]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
        if(vars::board[8]->label() == vars::board[0]->label() && vars::board[8]->label() == vars::board[4]->label()) {
            winner = true;
            if(vars::board[8]->label() == vars::player1) { whoWin = "Player 1"; }
            else { whoWin = "CPU"; }
        }
    }
    if(winner == false){
        int contador = 0;
        for(int i = 0; i < 9; i++){
            if(vars::board[i]->label() == vars::player1 || vars::board[i]->label() == vars::cpu) { contador++; }
        }
        if(contador==9){ empate = true; }
    }
    if(winner == true) {
        vars::isRunning = false;
        ended = true;
        vars::showing = true;
        if(whoWin == "Player 1") { vars::playerVictorys++; }
        else { vars::cpuVictorys++; }
        mainWin->EndGame();
        show_winner_message(whoWin);
    }
    if(empate == true) {
        vars::isRunning = false;
        ended = true;
        vars::showing = true;
        vars::draws++;
        mainWin->EndGame();
        show_draw_message();
    }
    return ended;
}


void MainWindow::ReMake() {
    if(vars::isRunning == false) { return; }
    pl1 = "PLAYER 1: " + vars::player1;
    cp = "CPU: " + vars::cpu;
    sturno = "TURNO: " + vars::turno;
    pv = "PLAYER VICTORIES: " + std::to_string(vars::playerVictorys);
    cv = "CPU VICTORIES: " + std::to_string(vars::cpuVictorys);
    dv = "EMPATES: " + std::to_string(vars::draws);
    vars::boxes[6]->label(pl1.c_str());
    vars::boxes[7]->label(cp.c_str());
    vars::boxes[8]->label(sturno.c_str());
    vars::boxes[10]->label(pv.c_str());
    vars::boxes[11]->label(cv.c_str());
    vars::boxes[12]->label(dv.c_str());
}

void MainWindow::MakeGame() {
    pl1 = "PLAYER 1: " + vars::player1;
    cp = "CPU: " + vars::cpu;
    sturno = "TURNO: " + vars::turno;
    pv = "PLAYER VICTORIES: " + std::to_string(vars::playerVictorys);
    cv = "CPU VICTORIES: " + std::to_string(vars::cpuVictorys);
    dv = "EMPATES: " + std::to_string(vars::draws);
    vars::boxes[0] = new Fl_Box(5, 5, 390, 490, "");
    vars::boxes[0]->box(FL_UP_BOX);
    vars::boxes[1] = new Fl_Box(100, 20, 5, 270, "");
    vars::boxes[1]->box(FL_UP_BOX);
    vars::boxes[1]->color(fl_rgb_color(1, 1, 1));
    vars::boxes[2] = new Fl_Box(200, 20, 5, 270, "");
    vars::boxes[2]->box(FL_UP_BOX);
    vars::boxes[2]->color(fl_rgb_color(1, 1, 1));
    vars::boxes[3] = new Fl_Box(20, 100, 270, 5, "");
    vars::boxes[3]->box(FL_UP_BOX);
    vars::boxes[3]->color(fl_rgb_color(1, 1, 1));
    vars::boxes[4] = new Fl_Box(20, 200, 270, 5, "");
    vars::boxes[4]->box(FL_UP_BOX);
    vars::boxes[4]->color(fl_rgb_color(1, 1, 1));
    vars::boxes[5] = new Fl_Box(400, 5, 295, 250, "");
    vars::boxes[5]->box(FL_UP_BOX);
    vars::boxes[6] = new Fl_Box(410, 20, 200, 40, "");
    vars::boxes[6]->box(FL_FLAT_BOX);
    vars::boxes[6]->labelfont(FL_BOLD + FL_ITALIC);
    vars::boxes[6]->labelsize(15);
    vars::boxes[6]->labeltype(FL_NORMAL_LABEL);
    vars::boxes[7] = new Fl_Box(410, 60, 200, 40, "");
    vars::boxes[7]->box(FL_FLAT_BOX);
    vars::boxes[7]->labelfont(FL_BOLD + FL_ITALIC);
    vars::boxes[7]->labelsize(15);
    vars::boxes[7]->labeltype(FL_NORMAL_LABEL);
    vars::boxes[8] = new Fl_Box(410, 100, 200, 40, "");
    vars::boxes[8]->box(FL_FLAT_BOX);
    vars::boxes[8]->labelfont(FL_BOLD + FL_ITALIC);
    vars::boxes[8]->labelsize(15);
    vars::boxes[8]->labeltype(FL_NORMAL_LABEL);
    vars::boxes[9] = new Fl_Box(400, 260, 295, 150, "");
    vars::boxes[9]->box(FL_UP_BOX);
    vars::boxes[10] = new Fl_Box(410, 270, 200, 40, pv.c_str());
    vars::boxes[10]->box(FL_FLAT_BOX);
    vars::boxes[10]->labelfont(FL_BOLD + FL_ITALIC);
    vars::boxes[10]->labelsize(15);
    vars::boxes[10]->labeltype(FL_NORMAL_LABEL);
    vars::boxes[11] = new Fl_Box(410, 310, 200, 40, cv.c_str());
    vars::boxes[11]->box(FL_FLAT_BOX);
    vars::boxes[11]->labelfont(FL_BOLD + FL_ITALIC);
    vars::boxes[11]->labelsize(15);
    vars::boxes[11]->labeltype(FL_NORMAL_LABEL);
    vars::boxes[12] = new Fl_Box(410, 350, 200, 40, dv.c_str());
    vars::boxes[12]->box(FL_FLAT_BOX);
    vars::boxes[12]->labelfont(FL_BOLD + FL_ITALIC);
    vars::boxes[12]->labelsize(15);
    vars::boxes[12]->labeltype(FL_NORMAL_LABEL);
    Fl_Button* startButton = new Fl_Button(450, 200, 200, 40, "Start Game");
    startButton->box(FL_UP_BOX);
    ButtonData* buttonData = new ButtonData();
    buttonData->id = 9;
    buttonData->mainWin = this;
    startButton->callback(buttonStartClick, buttonData); // Pass the MainWindow object as data
    this->add(vars::boxes[0]);
    this->add(vars::boxes[1]);
    this->add(vars::boxes[2]);
    this->add(vars::boxes[3]);
    this->add(vars::boxes[4]);
    this->add(vars::boxes[5]);
    this->add(vars::boxes[6]);
    this->add(vars::boxes[7]);
    this->add(vars::boxes[8]);
    this->add(vars::boxes[9]);
    this->add(vars::boxes[10]);
    this->add(vars::boxes[11]);
    this->add(vars::boxes[12]);
    this->add(startButton);
    int x = 15;
    int y = 15;
    int row = 0;
    int col = 0;
    for(int i = 0; i < 9; i++){
        col++;
        vars::board[i] = new Fl_Button(x, y, 80, 80, "");
        vars::board[i]->box(FL_NO_BOX);
        ButtonData* buttonData = new ButtonData();
        buttonData->id = i;
        buttonData->mainWin = this;
        vars::board[i]->callback(buttonClick, buttonData); // Pass the MainWindow object as data
        x+=100;
        if(col==3){ col = 0; row++; x = 15; y+=100; }
        this->add(vars::board[i]);
    }
}

void setSigns() {
    if(vars::isRunning == false) { return; }
    std::srand(std::time(0));
    // Define the range [min, max]
    int min = 1;
    int max = 20;
    // Generate a random number in the specified range
    int random_number = min + std::rand() % ((max + 1) - min);
    if(random_number<=10){
        vars::player1="O";
        vars::cpu="X";
        vars::turno="Player 1";
    }else{
        vars::player1="X";
        vars::cpu="O";
        vars::turno="CPU";
    }
}

void MainWindow::buttonClick(Fl_Widget* widget, void* data) {
    if(vars::turnoChanging == true) { return; }
    if(vars::isRunning == false) { return; }
    Fl_Button* clickedButton = dynamic_cast<Fl_Button*>(widget);
    if(vars::turno!="Player 1") { return; }
    if(vars::showing==true) { return; }
    Fl_Box* box = static_cast<Fl_Box*>(data);
    ButtonData* buttonData = static_cast<ButtonData*>(data); // Retrieve the ButtonData structure
    int id = buttonData->id;
    MainWindow* mainWin = buttonData->mainWin;
    bool changedTurno = false;
    if(clickedButton->label() == nullptr || strlen(clickedButton->label()) == 0){
        vars::board[id]->label(vars::player1.c_str());
        changedTurno = true;
    }
    if(mainWin->checkResult(widget, data) == false){
        if(changedTurno){ vars::turnoChanging = true; mainWin->changeTurno(widget, data); }
    }
}

void MainWindow::buttonStartClick(Fl_Widget* widget, void* data) {
    if(vars::isRunning == true) { return; }
    ButtonData* buttonData = static_cast<ButtonData*>(data); // Retrieve the ButtonData structure
    int id = buttonData->id;
    MainWindow* mainWin = buttonData->mainWin;
    vars::isRunning = true;
    setSigns();
    mainWin->ReMake();
    if(vars::turno=="CPU") { mainWin->cpuTurno(widget, data); }
}

void MainWindow::cpuTurno(Fl_Widget* widget, void* data){
    if(vars::isRunning == false) { return; }
    MainWindow* mainWin = static_cast<MainWindow*>(data);
    mainWin->ReMake();
    std::vector<int> freeBoardSpot;
    for(int i = 0; i < 9; i++){
        const char* label = vars::board[i]->label();
        if (label == nullptr || strlen(label) == 0) {
            freeBoardSpot.push_back(i);
        }
    }
    std::size_t size = freeBoardSpot.size();
    std::srand(static_cast<unsigned>(std::time(0))); // Seed random number generator only once in your program
    int random_number = std::rand() % size;
    int chosenSpot = freeBoardSpot[random_number];
    vars::board[chosenSpot]->label(vars::cpu.c_str());
    if(mainWin->checkResultCPU(widget, chosenSpot) == false){
        freeBoardSpot.clear();
        mainWin->changeTurno(widget, data);
    }
}

MainWindow::MainWindow(int width, int height, const char* title) : Fl_Window(width, height, title) {
    MakeGame();
}


int main(int argc, char** argv) {
    //Create the classes with the windows in them"
    MainWindow mainWindow(700, 500, "Tic Tac Toe");
    //to display the mainWindow
    mainWindow.show(argc, argv);

    // to run the program without this i can create everything but it won't run
    return Fl::run();
}
