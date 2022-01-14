/**
 * \file hangman.h
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

#ifndef HANGMAN_H_
#define HANGMAN_H_

#include <QMainWindow>
#include <QTranslator>

class QGraphicsEllipseItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QToolButton;

class Game;

namespace Ui {
class Hangman;
}

class Hangman : public QMainWindow {
  Q_OBJECT

 public:
    explicit Hangman(QWidget *pParent = nullptr);
    ~Hangman();

 signals:
    void checkLetter(const QByteArray &baLetter);

 protected:
    void changeEvent(QEvent *pEvent) override;

 private slots:
    void clickLetter();
    void updateWord(const QByteArray &baWord, const quint8 nWrong,
                    const quint16 nCorrectInRow, const quint16 nSumCorrect,
                    const quint16 nPlayedWords, const quint16 nQuantity);
    void showAnswer(const QString &sAnswer,
                    const QString &sWord = QLatin1String(""));
    void showInfoBox();

 private:
    void setupActions();
    static auto getSystemLanguage() -> QString;
    void loadLanguage(const QString &sLang);
    void createGallows();
    void newWord(const quint16 nCorrectInRow,
                 const quint16 nSumCorrect,
                 const quint16 nPlayedWords) const;
    void playedAll(const quint16 nCorrectInRow, const quint16 nSumCorrect,
                   const quint16 nPlayedWords, const quint16 nQuantity);

    Ui::Hangman *m_pUi;
    QTranslator m_translator;
    Game *m_pGame;
    QList<QToolButton*> m_pButtons;
    QGraphicsScene *m_pScene{};
    QGraphicsLineItem *m_pGallows1{};
    QGraphicsLineItem *m_pGallows2{};
    QGraphicsLineItem *m_pGallows3{};
    QGraphicsLineItem *m_pGallows4{};
    QGraphicsLineItem *m_pRope{};
    QGraphicsEllipseItem *m_pHead{};
    QGraphicsLineItem *m_pBody{};
    QGraphicsLineItem *m_pArmLeft{};
    QGraphicsLineItem *m_pArmRight{};
    QGraphicsLineItem *m_pLegLeft{};
    QGraphicsLineItem *m_pLegRight{};
};

#endif  // HANGMAN_H_
