#include "mainwindow.h"

#include <QVBoxLayout>
#include <QKeyEvent>
#include <QMenu>
#include <QMenuBar>
#include <QInputDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QFileDialog>
#include <QSettings>
#include <QTextCodec>
#include <QDebug>

#include <iostream>
#include <ctime>
#include <fstream>

#define UNDO_COUNT_TEXT "撤销次数："+QString::number(undoCount)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    gameArea = new GameArea;
    newGameButton = new QPushButton("新游戏");
    nameLabel = new QLabel("2048");
    scoreLabel = new QLabel("0");
    undoCountLabel = new QLabel("撤销次数：0");
    newGameAction = new QAction("新游戏");
    openAction = new QAction("打开");
    saveAction = new QAction("保存");
    saveAsAction = new QAction("另存为");
    cmdAction = new QAction("指令");
    helpCmdAction = new QAction("指令帮助");
    undoAction = new QAction("撤销");
    undoLockAction = new QAction("锁定撤销");
    updateContentAction = new QAction("更新内容");
    aboutQtAction = new QAction("关于Qt");
    aboutMeAction = new QAction("关于作者");
    memset(numbers, 0, sizeof(numbers));

    randomEngine.seed(time(nullptr));

    init_settings();
    init_ui();

    connect(newGameButton, SIGNAL(clicked()), this, SLOT(new_game()));
    connect(newGameAction, SIGNAL(triggered()), this, SLOT(new_game()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(save_as()));
    connect(cmdAction, SIGNAL(triggered()), this, SLOT(run_cmd()));
    connect(helpCmdAction, SIGNAL(triggered()), this, SLOT(show_cmd_help()));
    connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));
    connect(undoLockAction, SIGNAL(triggered(bool)), this, SLOT(set_undo_lock(bool)));
    connect(updateContentAction, SIGNAL(triggered()), this, SLOT(show_update_content()));
    connect(aboutQtAction, SIGNAL(triggered()), this, SLOT(about_qt()));
    connect(aboutMeAction, SIGNAL(triggered()), this, SLOT(about_me()));

    new_game();

    setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow()
= default;

void MainWindow::init_ui() {
    setFixedWidth(gameArea->frameSize + 20);
    setWindowTitle("2048Game");
#ifdef _CLion
    setWindowIcon(QIcon("D://C++Project//2048Game//2048.ico"));
#else
    setWindowIcon(QIcon(":/icon/2048.ico"));
#endif

    auto *widget = new QWidget;
    widget->setStyleSheet("background-color: rgb(250, 248, 239)");

    nameLabel->setStyleSheet("font-family: \"Yu Gothic UI Semibold\"; font-size: 50px; color: #776e65; font-weight: bold");
    newGameButton->setStyleSheet("font-family: \"Microsoft YaHei\"; font-size: 9px; color: #f9f6f2; font-weight: bold; background-color: #8f7a66; border-radius: 8px;");
    newGameButton->setFixedSize(60, 30);

    auto scoreWidget = new QWidget;
    auto scoreLayout = new QVBoxLayout;
    auto scoreNameLabel = new QLabel("分数");
    scoreWidget->setStyleSheet("background-color: rgb(187, 173, 160); border-radius: 4px;");
    scoreNameLabel->setStyleSheet("font-family: \"Microsoft YaHei\"; font-size: 10px; color: #eee4da; font-weight: bold");
    scoreNameLabel->setAlignment(Qt::AlignHCenter);
    scoreLabel->setStyleSheet("font-family: \"Microsoft JhengHei\"; font-size: 18px; color: #ffffff; font-weight: bold");
    scoreLabel->setAlignment(Qt::AlignHCenter);
    scoreLayout->addStretch();
    scoreLayout->addWidget(scoreNameLabel);
    scoreLayout->addWidget(scoreLabel);
    scoreLayout->addStretch();
    scoreWidget->setLayout(scoreLayout);
    scoreWidget->setFixedSize(90, 60);

    auto titleLayout = new QHBoxLayout;
    titleLayout->addWidget(nameLabel);
    titleLayout->addStretch(0);
    titleLayout->addWidget(newGameButton);
    titleLayout->addWidget(scoreWidget);

    auto widgetMainLayout = new QVBoxLayout;
    widgetMainLayout->addLayout(titleLayout);
    widgetMainLayout->addWidget(gameArea);
    widgetMainLayout->addStretch(0);
    widgetMainLayout->setContentsMargins(10, 5, 10, 5);

    widget->setLayout(widgetMainLayout);
    setCentralWidget(widget);

    auto fileMenu = menuBar()->addMenu("文件");
    fileMenu->addAction(newGameAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    newGameAction->setShortcut(QKeySequence::New);
    openAction->setShortcut(QKeySequence::Open);
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction->setShortcut(QKeySequence::SaveAs);

    auto operMenu = menuBar()->addMenu("操作");
    operMenu->addAction(cmdAction);
    operMenu->addAction(helpCmdAction);
    operMenu->addAction(undoAction);
    operMenu->addAction(undoLockAction);
    cmdAction->setShortcut(QKeySequence("Ctrl+R"));
    undoAction->setShortcut(QKeySequence::Undo);
    undoLockAction->setCheckable(true);

    auto aboutMenu = menuBar()->addMenu("关于");
    aboutMenu->addAction(updateContentAction);
    aboutMenu->addAction(aboutQtAction);
    aboutMenu->addAction(aboutMeAction);

    undoCountLabel->setFont(QFont("Microsoft YaHei"));
    statusBar()->addPermanentWidget(undoCountLabel);
}

void MainWindow::random_spawn_number() {
    int emptyCells[cellCount * cellCount][2];
    int emptyCount = 0;
    for (int row = 0; row < cellCount; ++row) {
        for (int column = 0; column < cellCount; ++column) {
            if (numbers[row][column] == 0) {
                emptyCells[emptyCount][0] = row;
                emptyCells[emptyCount][1] = column;
                emptyCount++;
            }
        }
    }
    if (emptyCount == 0) return;
    unsigned int randomIndex = std::uniform_int_distribution<unsigned>(0, emptyCount - 1)(randomEngine);
    int randomNumber;
    if (std::bernoulli_distribution(0.9)(randomEngine)) {
        randomNumber = 1;
    } else {
        randomNumber = 2;
    }
    numbers[emptyCells[randomIndex][0]][emptyCells[randomIndex][1]] = randomNumber;
    gameArea->add_spawn_animation(emptyCells[randomIndex][0], emptyCells[randomIndex][1], randomNumber);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
        auto key = event->key();
        if (key == Qt::Key_Up or key == Qt::Key_W) {
            //std::cout << "UP key pressed" << std::endl;
            up();
        } else if (key == Qt::Key_Down or key == Qt::Key_S) {
            //std::cout << "DOWN key pressed" << std::endl;
            down();
        } else if (key == Qt::Key_Left or key == Qt::Key_A) {
            //std::cout << "LEFT key pressed" << std::endl;
            left();
        } else if (key == Qt::Key_Right or key == Qt::Key_D) {
            right();
        }
    // output();
    QWidget::keyPressEvent(event);
}

void MainWindow::up() {
    gameArea->stop_animation();
    memset(isSpan, 0, sizeof(isSpan));
    NumbersStep step{};
    step.score = score;
    memcpy(step.numbers, numbers, sizeof(numbers));

    bool flag = false;
    for (int row = 1; row < cellCount; ++row) {
        for (int column = 0; column < cellCount; ++column) {
            if (numbers[row][column] == 0) continue;

            int tr = row - 1;
            for (; tr >= 0; --tr) if (numbers[tr][column] != 0) break;

            if (tr == -1) {
                flag = true;
                move_number(row, column, 0, column);
            } else if (try_span(row, column, tr, column)) {
                flag = true;
            } else {
                tr++;
                if (tr != row) {
                    move_number(row, column, tr, column);
                    flag = true;
                }
            }
        }
    }
    if (flag) {
        push_to_stack(step);
        random_spawn_number();
        gameArea->start_animation();
    }
}

void MainWindow::down() {
    gameArea->stop_animation();
    memset(isSpan, 0, sizeof(isSpan));
    NumbersStep step{};
    step.score = score;
    memcpy(step.numbers, numbers, sizeof(numbers));

    bool flag = false;
    for (int row = cellCount - 2; row >= 0; --row)
        for (int column = 0; column < cellCount; ++column) {
            if (numbers[row][column] == 0) continue;

            int tr = row + 1;
            for (; tr < cellCount; ++tr) if (numbers[tr][column] != 0) break;

            if (tr == cellCount) {
                flag = true;
                move_number(row, column, cellCount - 1, column);
            } else if (try_span(row, column, tr, column)) {
                flag = true;
            } else {
                tr--;
                if (tr != row) {
                    move_number(row, column, tr, column);
                    flag = true;
                }
            }
        }
    if (flag) {
        push_to_stack(step);
        random_spawn_number();
        gameArea->start_animation();
    }
}

void MainWindow::left() {
    gameArea->stop_animation();
    memset(isSpan, 0, sizeof(isSpan));
    NumbersStep step{};
    step.score = score;
    memcpy(step.numbers, numbers, sizeof(numbers));

    bool flag = false;
    for (int column = 1; column < cellCount; ++column) {
        for (int row = 0; row < cellCount; ++row) {
            if (numbers[row][column] == 0) continue;

            int tc = column - 1;
            for (; tc >= 0; --tc) if (numbers[row][tc] != 0) break;

            if (tc == -1) {
                move_number(row, column, row, 0);
                flag = true;
            } else if (try_span(row, column, row, tc)) {
                flag = true;
            } else {
                tc++;
                if (tc != column) {
                    move_number(row, column, row, tc);
                    flag = true;
                }
            }
        }
    }
    if (flag) {
        push_to_stack(step);
        random_spawn_number();
        gameArea->start_animation();
    }
}

void MainWindow::right() {
    gameArea->stop_animation();
    memset(isSpan, 0, sizeof(isSpan));
    NumbersStep step{};
    step.score = score;
    memcpy(step.numbers, numbers, sizeof(numbers));

    bool flag = false;
    for (int column = cellCount - 2; column >= 0; --column) {
        for (int row = 0; row < cellCount; ++row) {
            if (numbers[row][column] == 0) continue;

            int tc = column + 1;
            for (; tc < cellCount; ++tc) if (numbers[row][tc] != 0) break;
            if (tc == cellCount) {
                move_number(row, column, row, cellCount - 1);
                flag = true;
            } else if (try_span(row, column, row, tc)) {
                flag = true;
            } else {
                tc--;
                if (tc != column) {
                    move_number(row, column, row, tc);
                    flag = true;
                }
            }
        }
    }

    if (flag) {
        push_to_stack(step);
        random_spawn_number();
        gameArea->start_animation();
    }
}

void MainWindow::output() {
    for (auto & number : numbers) {
        for (int j : number) {
            printf("%5d ", j);
        }
        printf("\n");
    }
}

void MainWindow::spawn_number_without_animation(int row, int column, int number) {
    numbers[row][column] = number;
    gameArea->data[row][column] = number;
    gameArea->update();
}

bool MainWindow::try_span(int fr, int fc, int tr, int tc) {
    if (isSpan[tr][tc]) return false;
    if (numbers[fr][fc] == numbers[tr][tc]) {
        gameArea->add_move_animation(fr, fc, tr, tc, numbers[fr][fc]);
        int n = ++numbers[tr][tc];
        numbers[fr][fc] = 0;
        gameArea->add_spawn_animation(tr, tc, n);
        isSpan[tr][tc] = true;
        score += 1 << n;
        scoreLabel->setText(QString::number(score));
        if (first2048 and n == 11) {
            first2048 = false;
            gameArea->show_win_animation();
        } else if (n == 17) {
            gameArea->show_end_animation(tr, tc);
        }
        return true;
    }
    return false;
}

void MainWindow::move_number(int fr, int fc, int tr, int tc) {
    int n = numbers[fr][fc];
    numbers[tr][tc] = n;
    numbers[fr][fc] = 0;
    gameArea->add_move_animation(fr, fc, tr, tc, n);
}

void MainWindow::new_game() {
    gameArea->clear();
    memset(numbers, 0, sizeof(numbers));
    score = 0;
    scoreLabel->setText("0");
    undoAction->setEnabled(false);
    undoCount = 0;
    undoCountLabel->setText("撤销次数：0");
    undoStack.clear();
    first2048 = true;

    random_spawn_number();
    random_spawn_number();
    gameArea->start_animation();
}

void MainWindow::run_cmd() {
    bool ok;
    QString cmdName = QInputDialog::getText(this, "执行", "输入指令。", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    if (cmdName == "new_game") {
        new_game();
    } else if (cmdName == "random_spawn_number") {
        random_spawn_number();
        gameArea->start_animation();
    } else if (cmdName == "spawn_number") {
        QString args = QInputDialog::getText(this, "参数", "输入spawn_number的参数\n int row, int column, int number", QLineEdit::Normal, "", &ok);
        if (ok) {
            int row, column, number;
            sscanf(args.toStdString().c_str(), "%d %d %d", &row, &column, &number);
            if (row >= cellCount or column < 0) {
                QMessageBox::warning(this, "无效指令", "无效参数row：" + QString::number(row));
                return;
            }
            if (column >= cellCount or column < 0) {
                QMessageBox::warning(this, "无效指令", "无效参数column：" + QString::number(column));
                return;
            }
            if (number >= 18 or number < 0) {
                QMessageBox::warning(this, "无效指令", "无效参数number：" + QString::number(number));
                return;
            }
            spawn_number_without_animation(row, column, number);
        }
    } else if (cmdName == "set_score") {
        QString args = QInputDialog::getText(this, "参数", "输入set_score的参数\nint score", QLineEdit::Normal, "", &ok);
        if (ok) {
            int s = args.toInt(&ok);
            if (ok) {
                score = s;
                scoreLabel->setText(QString::number(s));
            } else {
                QMessageBox::warning(this, "无效指令", "无效参数score：" + args);
            }
        }
    } else if (cmdName == "up") {
        up();
    } else if (cmdName == "down") {
        down();
    } else if (cmdName == "left") {
        left();
    } else if (cmdName == "right") {
        right();
    } else if (cmdName == "fill_number") {
        QString args = QInputDialog::getText(this, "参数", "输入fill_number的参数\n int sr, int sc, int er, int ec, int number", QLineEdit::Normal, "", &ok);
        if (!ok) return;
        int sr, sc, er, ec, n;
        sscanf(args.toStdString().c_str(), "%d %d %d %d %d", &sr, &sc, &er, &ec, &n);
        if (sr < 0 or sr > 4) {
            QMessageBox::warning(this, "无效指令", "无效参数sr：" + QString::number(sr));
            return;
        }
        if (sc < 0 or sc > 4) {
            QMessageBox::warning(this, "无效指令", "无效参数sc：" + QString::number(sc));
            return;
        }
        if (er < 0 or er > 4) {
            QMessageBox::warning(this, "无效指令", "无效参数er：" + QString::number(er));
            return;
        }
        if (ec < 0 or ec > 4) {
            QMessageBox::warning(this, "无效指令", "无效参数ec：" + QString::number(ec));
            return;
        }
        if (n >= 18 or n < 0) {
            QMessageBox::warning(this, "无效指令", "无效参数n：" + QString::number(n));
            return;
        }
        fill_number(sr, sc, er, ec, n);
    } else if (cmdName == "set_random_seed") {
        QString args = QInputDialog::getText(this, "参数", "输入set_random_seed的参数\n int seed", QLineEdit::Normal, "", &ok);
        if (ok) {
            int seed = args.toInt(&ok);
            if (ok) {
                randomEngine.seed(seed);
            }
        }
    } else if (cmdName == "end"){
        int n = 1;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                spawn_number_without_animation(i , j, n++);
            }
        }
    } else if (cmdName == "LOVE") {
        QMessageBox::information(this, "LOVE", commandLoveText);
    } else if (cmdName == "about_me") {
        about_me();
    } else if (cmdName == "f2048") {
        fill_number(0, 0, 4, 4, 11);
    } else if (cmdName == "f2") {
        fill_number(0, 0, 4, 4, 1);
    } else if (cmdName == "clear") {
        fill_number(0, 0, 4, 4, 0);
    } else if (cmdName == "f8192") {
        fill_number(0, 0, 4, 4, 13);
    } else if (cmdName == "f131072") {
        fill_number(0, 0, 4, 4, 17);
    } else if (cmdName == "get_max") {
        QMessageBox::information(this, "最大值", commandGetMaxText);
    } else if (cmdName == "THANKS") {
        QMessageBox::information(this, "感谢", "感谢羊智凯的鼓励。");
    }
    else QMessageBox::warning(this, "无效指令", "无效指令：" + cmdName);
}

void MainWindow::show_update_content() {
    QMessageBox::about(this, "更新内容", "最后更新：" + updateDateText + "<br>更新内容：<br>" + updateContentText);
}

void MainWindow::about_qt() {
    QMessageBox::aboutQt(this, "关于Qt");
}

void MainWindow::about_me() {
    QMessageBox::about(this, "关于", "作者：自由");
}

void MainWindow::undo() {
    if (undoLock) return;
    if (undoStack.empty()) return;
    NumbersStep step = undoStack.back();
    undoStack.pop_back();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            spawn_number_without_animation(i, j, step.numbers[i][j]);
        }
    }
    score = step.score;
    undoCount++;
    undoCountLabel->setText("撤销次数："+QString::number(undoCount));
    scoreLabel->setText(QString::number(score));
    if (undoStack.empty()) undoAction->setEnabled(false);
}

void MainWindow::fill_number(int sr, int sc, int er, int ec, int number) {
    for (int r = sr; r < er; ++r) {
        for (int c = sc; c < ec; ++c) {
            spawn_number_without_animation(r, c, number);
        }
    }
}

void MainWindow::show_cmd_help() {
    QMessageBox::information(this, "指令帮助", commandHelpText);
}

void MainWindow::push_to_stack(NumbersStep step) {
    if (undoStack.size() >= 64) {
        undoStack.pop_front();
    }
    undoStack.push_back(step);
    undoAction->setEnabled(true);
}

bool MainWindow::write_file(const QString& filepath) {
    std::ofstream f;
    f.open(filepath.toLocal8Bit(), std::ios::binary);
    if (!f.is_open()) return false;
    fp = filepath;

    char _undoLock = (char)undoLock;
    f.write((char*)&score, sizeof(score));
    f.write((char*)&undoCount, sizeof(undoCount));
    f.write((char*)numbers, sizeof(numbers));
    f.write(&_undoLock, sizeof(_undoLock));
    f.close();

    statusBar()->showMessage("已保存文件到："+fp, 5000);
    return true;
}

bool MainWindow::read_file(const QString &filepath) {
    std::ifstream f;
    f.open(filepath.toLocal8Bit(), std::ios::binary);
    if (!f.is_open()) return false;
    fp = filepath;

    char _undoLock;
    f.read((char*)&score, sizeof(score));
    f.read((char*)&undoCount, sizeof(undoCount));
    f.read((char*)numbers, sizeof(numbers));
    f.read(&_undoLock, sizeof(_undoLock));

    set_undo_lock((bool)_undoLock);
    scoreLabel->setText(QString::number(score));
    bool first2048Flag = true;
    gameArea->stop_animation();
    for (int i = 0; i < cellCount; ++i) {
        for (int j = 0; j < cellCount; ++j) {
            if (numbers[i][j] == 0) {
                gameArea->data[i][j] = 0;
            } else {
                gameArea->add_spawn_animation(i, j, numbers[i][j]);
            }
            if (numbers[i][j] >= 11) {
                first2048Flag = false;
            }
        }
    }
    gameArea->start_animation();
    first2048 = first2048Flag;

    undoStack.clear();
    undoAction->setEnabled(false);

    statusBar()->showMessage("已打开文件："+fp, 5000);

    return true;
}

void MainWindow::open() {
    QString filepath = QFileDialog::getOpenFileName(this, "打开", "", "2048游戏存档(*.2048game)");
    if (filepath.isEmpty()) return;
    read_file(filepath);
}

void MainWindow::save() {
    if (fp.isEmpty()) save_as();
    else write_file(fp);
}

void MainWindow::save_as() {
    QString filepath = QFileDialog::getSaveFileName(this, "另存为", "", "2048游戏存档(*.2048game)");
    if (!filepath.isEmpty()) write_file(filepath);
}

void MainWindow::set_undo_lock(bool l) {
    undoLock = l;
    if (!l) {
        undoCountLabel->setText(UNDO_COUNT_TEXT);
        if (undoLockAction->isChecked()) undoLockAction->setChecked(false);
    } else {
        undoCountLabel->setText("撤销已锁定。");
        if (!undoLockAction->isChecked()) undoLockAction->setChecked(true);
    }
}

void MainWindow::init_settings() {
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    QStringList texts = settings.value("text/cellTexts").toStringList();
    for (int i = 0; i < 17; ++i) {
        gameArea->cellTexts[i + 1] = texts[i];
    }

    QStringList sizes = settings.value("style/sizes").toStringList();
    QString family = settings.value("style/family").toString();
    for (int i = 0; i < 17; ++i) {
        gameArea->cellTextFonts[i + 1] = QFont(family, sizes[i].toInt());
    }

    QStringList cellColors = settings.value("style/cellColors").toStringList();
    for (int i = 0; i < 18; ++i) {
        gameArea->cellBgBrushes[i] = QColor(cellColors[i]);
    }

    QStringList textColors = settings.value("style/textColors").toStringList();
    for (int i = 0; i < 18; ++i) {
        gameArea->cellTextColors[i + 1] = QColor(textColors[i]);
    }
    gameArea->reload_style();

    QSettings textSettings("text.ini", QSettings::IniFormat);
    textSettings.setIniCodec(QTextCodec::codecForName("UTF-8"));
    commandHelpText = textSettings.value("command/helpText").toString();
    commandLoveText = textSettings.value("command/loveText").toString();
    commandGetMaxText = textSettings.value("command/getMaxText").toString();
    updateDateText = textSettings.value("update/updateDateText").toString();
    updateContentText = textSettings.value("update/updateContentText").toString();
    gameArea->setTellHerText(textSettings.value("gamearea/tellHerText").toString());
}
