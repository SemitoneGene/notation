// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// System
#include <iostream>
#include <string>

// Qt
#include <QtWidgets/QtWidgets>

// Notation
#include "FileMediatorFactory.h"
#include "FontManager.h"
#include "FontTheme.h"
#include "Preferences.h"
#include "Score.h"
#include "ScoreCreationInfo.h"

// Local
#include "TestController.h"


komp::Score* gScore = nullptr;

static void
runFastTests()
{
    komp::TestController testController;
    testController.runTests(true);
    
    const auto message = testController.getTestRunResultMessage();
    std::cout << std::endl << message << std::endl;
    
    if (testController.getFailureCount() > 0 || testController.getErrorCount() > 0) {
        std::cout << std::endl << "Please fix the broken tests!" << std::endl;
    } else {
        std::cout << std::endl << "Success" << std::endl;
    }
}

static const constexpr auto SCALE = 2.0;

static QGraphicsView*
configureGraphicsView(QGraphicsScene* inScene)
{
    const auto view = new QGraphicsView(inScene, nullptr);
    view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    view->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    view->scale(SCALE, SCALE);
    view->horizontalScrollBar()->setValue(0);
    view->verticalScrollBar()->setValue(0);
    
    return view;
}

static void
configureMenuBar(QMainWindow& inWindow, QGraphicsView* inView)
{
    const auto menuBar = inWindow.menuBar();
    
    const auto fileMenu = menuBar->addMenu("File");
    const auto newAction = fileMenu->addAction("New");
    const auto openAction = fileMenu->addAction("Open");
    const auto saveAction = fileMenu->addAction("Save");
    const auto exitAction = fileMenu->addAction("Exit");

    const auto editMenu = menuBar->addMenu("Edit");
    const auto cutAction = editMenu->addAction("Cut");
    const auto copyAction = editMenu->addAction("Copy");
    const auto pasteAction = editMenu->addAction("Paste");
    
    const auto scoreMenu = menuBar->addMenu("Score");
    const auto viewMenu = menuBar->addMenu("View");
    const auto windowsMenu = menuBar->addMenu("Windows");
    
    const auto debugMenu = menuBar->addMenu("Debug");
    const auto runFastTestAction = debugMenu->addAction("Run Fast Tests");
    const auto outlinesAction = debugMenu->addAction("Outlines");
    outlinesAction->setCheckable(true);
    
    // Zoom in action
    const auto zoomInAction = viewMenu->addAction("Zoom In");
    zoomInAction->setShortcuts(QKeySequence::ZoomIn);
    QObject::connect(zoomInAction, &QAction::triggered, [inView]() {
        inView->scale(1.2, 1.2);
    });

    // Zoom out action
    const auto zoomOutAction = viewMenu->addAction("Zoom Out");
    zoomOutAction->setShortcuts(QKeySequence::ZoomOut);
    QObject::connect(zoomOutAction, &QAction::triggered, [inView]() {
        inView->scale(0.8, 0.8);
    });

    // Reset zoom action
    const auto resetZoomAction = viewMenu->addAction("Reset Zoom");
    resetZoomAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0));
    QObject::connect(resetZoomAction, &QAction::triggered, [inView]() {
        inView->resetTransform();
    });

    QObject::connect(outlinesAction, &QAction::triggered, [outlinesAction]() {
        auto& prefs = komp::Preferences::instance();
        prefs.showOutlines(outlinesAction->isChecked());
        gScore->update();
    });
    
    QObject::connect(exitAction, &QAction::triggered, [&]() {
        inWindow.close();
    });

    QObject::connect(runFastTestAction, &QAction::triggered, [&]() {
        runFastTests();
    });
}

static const constexpr auto WIDTH = 1280.0;
static const constexpr auto HEIGHT = 800.0;

int
main (int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    auto& fontManager = komp::FontManager::instance();
    fontManager.configure(std::string(RESOURCE_DIR) + "Fonts/");

    const auto binaryPath = std::filesystem::absolute(std::filesystem::current_path());
    const auto parentDir = binaryPath.parent_path();
    const auto filePath = parentDir / "Haydn.xml";
    auto fileMediator = komp::FileMediatorFactory::create(komp::FileMediatorType::MusicXML);
    
    auto score = fileMediator->loadFile(filePath.string());
    
    //auto score = komp::Score::create(komp::defaultScoreInfo());
    score->loaded();
    score->layout();
    
    gScore = score.get();
    
    QMainWindow scoreWindow;
    scoreWindow.resize(WIDTH, HEIGHT);
    scoreWindow.setWindowTitle("Komp");
    
    const auto scene = static_cast<QGraphicsScene*>(score->getContext().getLayer());
    const QRectF rect(0, 0, WIDTH, HEIGHT);
    scene->setSceneRect(rect);
    
    const auto graphicsView = configureGraphicsView(scene);
    configureMenuBar(scoreWindow, graphicsView);
    
    scoreWindow.setCentralWidget(graphicsView);
    scoreWindow.show();
    
    score->draw();
    
    return app.exec();
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
