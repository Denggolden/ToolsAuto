#include "DialogBase.h"

DialogBase::DialogBase(QWidget *parent)
    : QDialog{parent}
{}

void DialogBase::InitClass()
{
    qDebug()<<"DialogBase::InitClass()";
}
