#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <random>
#include <QPushButton>
#include <QLabel>
#include <QAction>
#include <deque>

#include "GameArea.h"

struct NumbersStep{
    int numbers[4][4];
    int score;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void keyPressEvent(QKeyEvent *event) override;

    GameArea *gameArea;
    QPushButton *newGameButton;
    QLabel *nameLabel;
    QLabel *scoreLabel;
    QLabel *undoCountLabel;
    QAction *newGameAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *undoLockAction;
    QAction *undoAction;
    QAction *cmdAction;
    QAction *helpCmdAction;
    QAction *updateContentAction;
    QAction *aboutQtAction;
    QAction *aboutMeAction;

public slots:
    void up();
    void down();
    void left();
    void right();

    void new_game();
    void run_cmd();
    void show_cmd_help();
    void undo();
    void show_update_content();
    void about_qt();
    void about_me();

    void open();
    void save();
    void save_as();
    void set_undo_lock(bool l);

private:
    void init_ui();
    void init_settings();

    void random_spawn_number();
    bool try_span(int fr, int fc, int tr, int tc);

    void output();
    void spawn_number_without_animation(int row, int column, int number);
    void move_number(int fr, int fc, int tr, int tc);
    void fill_number(int sr, int sc, int er, int ec, int number);
    bool write_file(const QString& filepath);
    bool read_file(const QString& filepath);

    int cellCount = 4;
    int numbers[4][4];
    bool isSpan[4][4];
    int score = 0;
    int undoCount = 0;
    bool undoLock = false;
    bool first2048 = true;
    QString fp;
    std::default_random_engine randomEngine;

    QString commandHelpText;
    QString commandLoveText;
    QString commandGetMaxText;
    QString updateDateText;
    QString updateContentText;

    std::deque<NumbersStep> undoStack;
    void push_to_stack(NumbersStep step);
};
#endif // MAINWINDOW_H
