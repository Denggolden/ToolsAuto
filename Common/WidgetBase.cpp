#include "WidgetBase.h"
#include <QDebug>

WidgetBase::WidgetBase(QWidget *parent) :
    QWidget(parent)
{

}

WidgetBase::~WidgetBase()
{

}

void WidgetBase::InitClass()
{
    qDebug()<<"WidgetBase::InitClass()";
}
