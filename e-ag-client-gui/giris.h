/*****************************************************************************
 *   Copyright (C) 2020 by Bayram KARAHAN                                    *
 *   <bayramk@gmail.com>                                                     *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 3 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .          *
 *****************************************************************************/
#include<QCheckBox>
#include<QProcess>
QWidget *MainWindow::giris()
{
    QWidget *girisPage=new QWidget();
    /*******************************************************/
    auto  screenbtn = new QPushButton(girisPage);
    screenbtn->setFixedSize(QSize(boy*60,boy*60));
    screenbtn->setIconSize(QSize(boy*60,boy*60));
    screenbtn->setAutoFillBackground(true);
    screenbtn->setIcon(QIcon(":/icons/e-ag.svg"));
    screenbtn->setFlat(true);


    auto *sat1=new QLabel(girisPage);
    sat1->setText("e-ag-gui istemci Uygulamasıdır.");
      QFont ff( "Arial", 8, QFont::Normal);
    sat1->setFont(ff);

    auto layout = new QGridLayout(girisPage);
    layout->setContentsMargins(0, 0, 0,0);
   // layout->setVerticalSpacing(5);
    //layout->setColumnMinimumWidth(0, 37);
    layout->addWidget(screenbtn, 5,0,1,2,Qt::AlignHCenter);

    layout->addWidget(sat1,10,0,1,2);
   // layout->addWidget(sat2,15,0,1,2);
    //layout->addWidget(sat3,20,0,1,2);

   // layout->setColumnStretch(6, 255);


    return girisPage;
}
#ifndef GIRIS_H
#define GIRIS_H

#endif // GIRIS_H
