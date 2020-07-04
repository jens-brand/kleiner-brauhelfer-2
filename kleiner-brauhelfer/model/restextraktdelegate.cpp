#include "restextraktdelegate.h"
#include "dialogs/dlgrestextrakt.h"
#include "modelschnellgaerverlauf.h"
#include "modelhauptgaerverlauf.h"
#include "brauhelfer.h"

extern Brauhelfer* bh;

RestextraktDelegate::RestextraktDelegate(bool hauptgaerung, QObject *parent) :
    DoubleSpinBoxDelegate(1, 0.0, 100.0, 0.1, false, parent),
    mHauptgaerung(hauptgaerung)
{
}

QWidget* RestextraktDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    if (mReadonly)
        return nullptr;
    double T;
    double re = index.data().toDouble();
    double sw = bh->sud()->getSWIst();
    if (mHauptgaerung)
        T = index.sibling(index.row(), ModelHauptgaerverlauf::ColTemp).data().toDouble();
    else
        T = index.sibling(index.row(), ModelSchnellgaerverlauf::ColTemp).data().toDouble();
    DlgRestextrakt* w = new DlgRestextrakt(re, sw, T, parent);
    return w;
}

void RestextraktDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    DlgRestextrakt *w = static_cast<DlgRestextrakt*>(editor);
    w->setValue(index.data(Qt::EditRole).toDouble());
    if (mHauptgaerung)
        w->setTemperatur(index.sibling(index.row(), ModelHauptgaerverlauf::ColTemp).data().toDouble());
    else
        w->setTemperatur(index.sibling(index.row(), ModelSchnellgaerverlauf::ColTemp).data().toDouble());
}

void RestextraktDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    DlgRestextrakt *w = static_cast<DlgRestextrakt*>(editor);
    if (w->result() == QDialog::Accepted)
    {
        model->setData(index, w->value(), Qt::EditRole);
        if (mHauptgaerung)
            model->setData(index.sibling(index.row(), ModelHauptgaerverlauf::ColTemp), w->temperatur(), Qt::EditRole);
        else
            model->setData(index.sibling(index.row(), ModelSchnellgaerverlauf::ColTemp), w->temperatur(), Qt::EditRole);
    }
}

void RestextraktDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(editor)
    Q_UNUSED(option)
    Q_UNUSED(index)
}
