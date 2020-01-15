#pragma once

#include <QComboBox>
#include <QSpinBox>
#include <QPointer>

#include <QDialog>
#include <QDialogButtonBox>
#include <QListWidget>

#include <QMainWindow>

#include "documentarea.h"

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QStatusBar;

class CountSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    explicit CountSpinBox( QWidget *parent = nullptr );

private:
    void initialize();

};

class SizeSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    explicit SizeSpinBox( QWidget *parent = nullptr );

private:
    void initialize();

};

class RescaleSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    explicit RescaleSpinBox( QWidget *parent = nullptr );

private:
    void initialize();

};

class FilesListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit FilesListWidget( QWidget *parent = nullptr );

public slots:
    void removeSelected();

private:
    void initialize();
};

class DocumentBase : public QWidget
{
    Q_OBJECT

public:
    explicit DocumentBase( QWidget *widget, QWidget* parent = nullptr );

    void setWidget( QWidget *widget );
    QWidget *widget() const;

protected:
    QVBoxLayout *m_layout;

    QWidget *m_widget;

private:
    void initialize();

};

class DialogBase : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBase( QWidget *parent = nullptr );
    explicit DialogBase( const QDialogButtonBox::StandardButtons buttons = QDialogButtonBox::NoButton, QWidget *parent = nullptr );

    void setWidget( QWidget *widget );

protected:
    QPointer< QVBoxLayout > m_layout;
    QPointer< QWidget > m_widget;
    QPointer< QDialogButtonBox > m_buttons;

private:
    void initialize();
    void initialize( const QDialogButtonBox::StandardButtons buttons );

};

class MainWindowBase : public QMainWindow
{
public:
    explicit MainWindowBase( QWidget *parent = nullptr );

protected:
    QPointer< QStatusBar > m_statusBar;

    void setupStatusBar();

private:
    void initialize();

};

class DocumentMainWindow : public MainWindowBase
{
    Q_OBJECT

public:
    explicit DocumentMainWindow( QWidget *parent = nullptr );

    void addDocument( DocumentBase *document );

    DocumentBase *currentDocument() const;

protected:
    QPointer< DocumentArea > m_documentArea;

    template <class T> T* getCurrentDocument() const;

    void setupDocuments();

private:
    void initialize();

};

class FolderLine : public QWidget
{
    Q_OBJECT

public:
    explicit FolderLine( const QString &label, QWidget *parent = nullptr );

    void setLabel( const QString label );
    QString label() const;

    void setPath( const QString &value );
    QString path() const;

public slots:
    void choiceDirectoryDialog();

protected:
    QPointer< QLabel > m_label;
    QPointer< QLineEdit > m_path;
    QPointer< QPushButton > m_button;

private:
    void initialize();
};

#include "supportwidgets.inl"
