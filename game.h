/**
 * \file game.h
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

#ifndef GAME_H_
#define GAME_H_

#include <QString>
#include <QObject>

class Game : public QObject {
  Q_OBJECT

 public:
    explicit Game(const QString &sRessource, QObject *pParent = nullptr);
    static const uint MAXTRIES = 7;
    static const uint NEWRORD = 111;
    static const uint PLAYEDALL = 112;

 public slots:
    void newGame();
    void checkLetter(const QByteArray &baLetter);

 signals:
    void updateWord(const QByteArray &baWord, const quint8 nWrong,
                    const quint16 nCorrectInRow, const quint16 nSumCorrect,
                    const quint16 nPlayedWords, const quint16 nQuantity);
    void showAnswer(const QString &sAnswer,
                    const QString &sWord = QLatin1String(""));

 private slots:
    void nextWord();

 private:
    void loadWordlist(const QString &sRessource);

    QStringList m_sListWords;
    QByteArray m_baWord;
    QByteArray m_baShownWord;
    QString m_sAnswer;
    quint8 m_nCntFalse{};
    quint16 m_nQuantity;
    quint16 m_nPlayedWords{};
    quint16 m_nSumCorrect{};
    quint16 m_nCorrectInRow{};
};

#endif  // GAME_H_
