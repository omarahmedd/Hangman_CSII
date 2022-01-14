/**
 * \file hangman.cpp
 *
 * \section LICENSE
 *
 * Copyright (C) 2008-2021 Thorsten Roth
 *
 * This file is part of Hangman.
 *
 * Hangman is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hangman is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hangman.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "./hangman.h"

#include <QDebug>
#include <QFile>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QToolButton>

#include "./game.h"
#include "ui_hangman.h"

Hangman::Hangman(QWidget *pParent)
  : QMainWindow(pParent),
    m_pUi(new Ui::Hangman) {
  m_pUi->setupUi(this);
  this->loadLanguage(Hangman::getSystemLanguage());
  m_pGame = new Game(QStringLiteral(":/data.dat"));
  this->setupActions();

  connect(m_pGame, &Game::updateWord, this, &Hangman::updateWord);
  connect(m_pGame, &Game::showAnswer, this, &Hangman::showAnswer);

  QFile stylefile(QStringLiteral(":/hangman.qss"));
  if (stylefile.open(QFile::ReadOnly)) {
    QString sStyleSheet = QString::fromLatin1(stylefile.readAll());
    this->setStyleSheet(sStyleSheet);
  } else {
    qWarning() << stylefile.fileName() << "style sheet could not be loaded!";
  }

  QFont font;
  font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
  font.setPixelSize(20);
  m_pUi->lbl_Word->setFont(font);
  font.setLetterSpacing(QFont::PercentageSpacing, 100);
  font.setPixelSize(13);
  m_pUi->lbl_CptCorrectInRow->setFont(font);
  m_pUi->lbl_CorrectInRow->setFont(font);
  m_pUi->lbl_CptCorrectPercent->setFont(font);
  m_pUi->lbl_CorrectPercent->setFont(font);

  this->createGallows();
  m_pGame->newGame();
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

Hangman::~Hangman() {
  delete m_pGame;
  delete m_pUi;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

void Hangman::setupActions() {
  // Menu
  m_pUi->action_NewGame->setShortcut(QKeySequence::New);
  connect(m_pUi->action_NewGame, &QAction::triggered, m_pGame, &Game::newGame);
  connect(m_pUi->action_Info, &QAction::triggered, this, &Hangman::showInfoBox);
  m_pUi->action_Quit->setShortcut(QKeySequence::Quit);
  connect(m_pUi->action_Quit, &QAction::triggered, this, &Hangman::close);

  for (int i = 0; i < 26; ++i) {  // Loop through ASCII A-Z (65 - 90)
    m_pButtons.append(new QToolButton(this));
    connect(m_pButtons.last(), &QToolButton::clicked,
            this, &Hangman::clickLetter);

    m_pButtons.last()->setText(QString(static_cast<char>(i + 65)));
    m_pButtons.last()->setFocusPolicy(Qt::NoFocus);
    m_pButtons.last()->setMinimumHeight(33);
    m_pButtons.last()->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Fixed);

    if (i < 24) {
      m_pUi->grid_Buttons->addWidget(m_pButtons.last(), i / 6, i % 6);
    } else if (24 == i) {
      m_pUi->grid_Buttons->addWidget(m_pButtons.last(), i / 6, 2);
    } else {
      m_pUi->grid_Buttons->addWidget(m_pButtons.last(), i / 6, 3);
    }
  }

  connect(this, &Hangman::checkLetter, m_pGame, &Game::checkLetter);
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

void Hangman::createGallows() {
  m_pScene = new QGraphicsScene(this);
  m_pUi->graphicsView->setScene(m_pScene);
  m_pUi->graphicsView->setRenderHints(
        QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

  QPen penGallows(QColor(46, 52, 54));
  penGallows.setWidth(10);
  m_pGallows1 = m_pScene->addLine(110, 235, 225, 235, penGallows);  // Base
  m_pGallows2 = m_pScene->addLine(165, 235, 165, 5, penGallows);    // Pillar
  m_pGallows2 = m_pScene->addLine(165, 5, 50, 5, penGallows);       // Beam
  m_pGallows2 = m_pScene->addLine(112, 8, 162, 58, penGallows);     // Brace

  QPen penRope(QColor(143, 89, 2));
  penRope.setWidth(7);
  m_pRope = m_pScene->addLine(56, 4, 56, 33, penRope);
  m_pRope->setVisible(false);

  QPen penBody(QColor(85, 87, 83));
  penBody.setWidth(5);
  m_pHead = m_pScene->addEllipse(38, 37, 36, 36, penBody);
  m_pHead->setVisible(false);
  m_pBody = m_pScene->addLine(56, 74, 56, 167, penBody);
  m_pBody->setVisible(false);
  m_pArmLeft = m_pScene->addLine(55, 90, 36, 130, penBody);
  m_pArmLeft->setVisible(false);
  m_pArmRight = m_pScene->addLine(57, 90, 76, 130, penBody);
  m_pArmRight->setVisible(false);
  m_pLegLeft = m_pScene->addLine(55, 169, 36, 209, penBody);
  m_pLegLeft->setVisible(false);
  m_pLegRight = m_pScene->addLine(57, 169, 76, 209, penBody);
  m_pLegRight->setVisible(false);
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

void Hangman::clickLetter() {
  auto *tmpBtn = qobject_cast<QToolButton *>(sender());
  if (tmpBtn != nullptr) {
    tmpBtn->setEnabled(false);
    emit checkLetter(tmpBtn->text().toLatin1());
  }
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

void Hangman::updateWord(const QByteArray &baWord,
                         const quint8 nWrong,
                         const quint16 nCorrectInRow,
                         const quint16 nSumCorrect,
                         const quint16 nPlayedWords,
                         const quint16 nQuantity) {
  m_pUi->lbl_Word->setText(QString::fromLatin1(baWord));

  switch (nWrong) {
    case 1: m_pRope->setVisible(true);
      break;
    case 2: m_pHead->setVisible(true);
      break;
    case 3: m_pBody->setVisible(true);
      break;
    case 4: m_pArmLeft->setVisible(true);
      break;
    case 5: m_pArmRight->setVisible(true);
      break;
    case 6: m_pLegLeft->setVisible(true);
      break;
    case 7: m_pLegRight->setVisible(true);
      break;
    case Game::NEWRORD: this->newWord(nCorrectInRow, nSumCorrect, nPlayedWords);
      break;
    case Game::PLAYEDALL: this->playedAll(nCorrectInRow, nSumCorrect,
                                          nPlayedWords, nQuantity);
      break;
    default: break;
  }
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

void Hangman::newWord(const quint16 nCorrectInRow,
                      const quint16 nSumCorrect,
                      const quint16 nPlayedWords) const {
  for (auto &button : m_pButtons) {
    button->setEnabled(true);
  }
  m_pUi->lbl_CorrectInRow->setText(QString::number(nCorrectInRow));
  m_pUi->lbl_CorrectPercent->setText(
        QString::number(nSumCorrect) + " / " + QString::number(nPlayedWords));

  quint8 nPercent = 0;
  if (nPlayedWords > 0) {
    nPercent = static_cast<quint8>(nSumCorrect * 100 / nPlayedWords);
  }
  m_pUi->lbl_CptCorrectPercent->setToolTip(QString::number(nPercent) + " %");
  m_pUi->lbl_CorrectPercent->setToolTip(QString::number(nPercent) + " %");

  m_pRope->setVisible(false);
  m_pHead->setVisible(false);
  m_pBody->setVisible(false);
  m_pArmLeft->setVisible(false);
  m_pArmRight->setVisible(false);
  m_pLegLeft->setVisible(false);
  m_pLegRight->setVisible(false);
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

void Hangman::playedAll(const quint16 nCorrectInRow,
                        const quint16 nSumCorrect,
                        const quint16 nPlayedWords,
                        const quint16 nQuantity) {
  for (auto &button : m_pButtons) {
    button->setEnabled(false);
  }
  m_pUi->lbl_CorrectInRow->setText(QString::number(nCorrectInRow));
  m_pUi->lbl_CorrectPercent->setText(
        QString::number(nSumCorrect) + " / " + QString::number(nPlayedWords));

  auto nPercentCorrect = static_cast<quint8>(nSumCorrect * 100 / nPlayedWords);
  QMessageBox::information(this, tr("All words used!"),
                           tr("You played all words!\n\n"
                              "You guessed %1 of %2 words.\n"
                              "This corresponds to a quote of %3 %")
                           .arg(QString::number(nSumCorrect),
                                QString::number(nQuantity),
                                QString::number(nPercentCorrect)));

  m_pRope->setVisible(false);
  m_pHead->setVisible(false);
  m_pBody->setVisible(false);
  m_pArmLeft->setVisible(false);
  m_pArmRight->setVisible(false);
  m_pLegLeft->setVisible(false);
  m_pLegRight->setVisible(false);
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

void Hangman::showAnswer(const QString &sAnswer, const QString &sWord) {
  QString sMeaning = QLatin1String("");
  if (!sAnswer.isEmpty()) {
    sMeaning = "\n" + QString(tr("It means: %1").arg(sAnswer));
  }

  if (sWord.isEmpty()) {
    QMessageBox::information(this, tr("Won"),
                             tr("You guessed the word!") + "\n" + sMeaning);
  } else {
    QMessageBox::information(this, tr("Lost"),
                             tr("Unfortunately you have lost!") + "\n\n" +
                             tr("The word was: %1").arg(sWord) + sMeaning);
  }
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

auto Hangman::getSystemLanguage() -> QString {
#ifdef Q_OS_UNIX
    QByteArray lang = qgetenv("LANG");
    if (!lang.isEmpty()) {
      return QLocale(QString::fromLatin1(lang)).name();
    }
#endif
    return QLocale::system().name();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void Hangman::loadLanguage(const QString &sLang) {
  qApp->removeTranslator(&m_translator);
  if (m_translator.load(":/" + qApp->applicationName().toLower() +
                        "_" + sLang + ".qm")) {
    qApp->installTranslator(&m_translator);
  } else {
    qWarning() << "Could not load translation :/" +
                  qApp->applicationName().toLower() + "_" + sLang + ".qm";
  }
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

void Hangman::changeEvent(QEvent *pEvent) {
  if (nullptr != pEvent) {
    if (QEvent::LanguageChange == pEvent->type()) {
      m_pUi->retranslateUi(this);
    }
  }
  QMainWindow::changeEvent(pEvent);
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

void Hangman::showInfoBox() {
  QMessageBox::about(
        this, QStringLiteral("Info"),
        QString::fromLatin1("<center>"
                            "<big><b>%1 %2</b></big><br />"
                            "%3<br />"
                            "<small>%4</small><br /><br />"
                            "%5<br />"
                            "%6<br />"
                            "</center>"
                            "%7")
        .arg(qApp->applicationName(),
             qApp->applicationVersion(),
             QStringLiteral(APP_DESC),
             QStringLiteral(APP_COPY),
             tr("URL") +
             ": <a href=\"https://github.com/ElTh0r0/hangman_qt\">"
             "https://github.com/ElTh0r0/hangman_qt</a>",
             tr("License") +
             ": <a href=\"https://www.gnu.org/licenses/gpl-3.0.html\">"
             "GNU General Public License Version 3</a>",
             "<i>" + tr("Translations") +
             "</i><br />"
             "&nbsp;&nbsp;- Dutch: Vistaus<br />"
             "&nbsp;&nbsp;- German: ElThoro<br />"
             "&nbsp;&nbsp;- Italian: davi92"));
}
