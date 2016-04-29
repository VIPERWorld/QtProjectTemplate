#include "MainWindow.h"
#include "TestItem.h"
#include "TestTreeModel.h"
#include "TestTreeSelectionModel.h"
#include "TestTreeView.h"
#include "OutputWidget.h"
#include "DirUtils.h"

#include <QTabWidget>
#include <QSplitter>

MainWindow::MainWindow()
{
    // Get root/tests/unit directory
    QDir unitDir = DirUtils::rootDir();
    DirUtils::cd(unitDir, "tests");
    DirUtils::cd(unitDir, "unit");

    // Get root/tests/unit out directory
    QDir unitOutDir = DirUtils::rootOutDir();
    DirUtils::cd(unitOutDir, "tests");
    DirUtils::cd(unitOutDir, "unit");

    // Test tree model
    testTreeModel_ = new TestTreeModel(unitDir, unitOutDir, this);

    // Test tree selection model
    testTreeSelectionModel_ = new TestTreeSelectionModel(testTreeModel_, this);

    // Test tree view
    testTreeView_ = new TestTreeView();
    testTreeView_->setModel(testTreeModel_);
    testTreeView_->setSelectionModel(testTreeSelectionModel_);

    // Output widgets
    relevantOutputWidget_ = new OutputWidget();
    compileOutputWidget_ = new OutputWidget();
    runOutputWidget_ = new OutputWidget();
    QTabWidget * outputWidgets = new QTabWidget();
    outputWidgets->addTab(relevantOutputWidget_, "Relevant Output");
    outputWidgets->addTab(compileOutputWidget_, "Compile Output");
    outputWidgets->addTab(runOutputWidget_, "Run Output");

    // Update output when current item changes
    connect(testTreeSelectionModel_, &TestTreeSelectionModel::currentTestItemChanged,
            this, &MainWindow::onCurrentTestItemChanged_);

    // Initialize current item
    testTreeView_->setCurrentIndex(testTreeModel_->index(0, 0));

    // Layout
    QSplitter * splitter = new QSplitter();
    splitter->addWidget(testTreeView_);
    splitter->addWidget(outputWidgets);
    splitter->setCollapsible(0, false);
    splitter->setCollapsible(1, false);
    setCentralWidget(splitter);

    // Set sensible sizes and proportions
    resize(1200, 650);
    splitter->setStretchFactor(0,1);
    splitter->setStretchFactor(1,2);
}

void MainWindow::onCurrentTestItemChanged_(TestItem * current, TestItem * previous)
{
    if (previous)
        previous->disconnect(this);

    if (current)
        connect(current, &TestItem::outputChanged,
                this, &MainWindow::updateOutput_);

    updateOutput_();
}

void MainWindow::updateOutput_()
{
    TestItem * currentTestItem = testTreeSelectionModel_->currentTestItem();

    if (currentTestItem)
    {
        relevantOutputWidget_->setOutput(currentTestItem->output());
        compileOutputWidget_->setOutput(currentTestItem->compileOutput());
        runOutputWidget_->setOutput(currentTestItem->runOutput());
    }
    else
    {
        relevantOutputWidget_->setOutput("");
        compileOutputWidget_->setOutput("");
        runOutputWidget_->setOutput("");
    }
}
