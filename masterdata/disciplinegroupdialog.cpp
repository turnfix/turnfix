#include "disciplinegroupdialog.h"
#include "model/entity/discipline.h"
#include "model/entity/disciplinegroup.h"
#include "model/entitymanager.h"
#include "model/repository/disciplinegrouprepository.h"
#include "model/view/disciplinegroupitemmodel.h"
#include "model/view/disciplinemodel.h"
#include "ui_disciplinegroupdialog.h"

DisciplineGroupDialog::DisciplineGroupDialog(DisciplineGroup *disciplineGroup,
                                             EntityManager *em,
                                             QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DisciplineGroupDialog)
    , m_disciplineGroup(disciplineGroup)
    , m_em(em)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    connect(ui->but_add, SIGNAL(clicked()), this, SLOT(addItem()));
    connect(ui->but_del, SIGNAL(clicked()), this, SLOT(removeItem()));
    connect(ui->but_up, SIGNAL(clicked()), this, SLOT(moveUp()));
    connect(ui->but_down, SIGNAL(clicked()), this, SLOT(moveDown()));
    connect(ui->bbx_done, SIGNAL(rejected()), this, SLOT(close()));
    connect(ui->bbx_done, SIGNAL(accepted()), this, SLOT(save()));

    if (m_disciplineGroup == nullptr) {
        m_disciplineGroup = new DisciplineGroup();
    }

    m_disciplineModel = new DisciplineModel(m_em, this);
    m_disciplineModel->fetchDisciplines();
    ui->tbl_all->setModel(m_disciplineModel);

    m_itemModel = new DisciplineGroupItemModel(m_em, this);
    m_itemModel->fetchItems(m_disciplineGroup);
    ui->tbl_group->setModel(m_itemModel);

    QList<QHeaderView::ResizeMode> resizeModes = {QHeaderView::Stretch,
                                                  QHeaderView::ResizeToContents};
    QList<int> sizes = {300, 250};

    for (int i = 0; i < 2; i++) {
        ui->tbl_all->horizontalHeader()->setSectionResizeMode(i, resizeModes.at(i));
        ui->tbl_all->horizontalHeader()->resizeSection(i, sizes.at(i));
        ui->tbl_group->horizontalHeader()->setSectionResizeMode(i, resizeModes.at(i));
        ui->tbl_group->horizontalHeader()->resizeSection(i, sizes.at(i));
    }

    ui->txt_name->setText(m_disciplineGroup->name());
    ui->txt_comment->setText(m_disciplineGroup->comment());
}

DisciplineGroupDialog::~DisciplineGroupDialog()
{
    delete ui;
}

void DisciplineGroupDialog::addItem()
{
    if (ui->tbl_all->selectionModel()->selectedIndexes().empty())
        return;
    m_itemModel->addItem(qvariant_cast<Discipline *>(
        m_disciplineModel->data(ui->tbl_all->currentIndex(), TF::ObjectRole)));
}

void DisciplineGroupDialog::removeItem()
{
    m_itemModel->removeItem(ui->tbl_group->currentIndex());
}

void DisciplineGroupDialog::moveUp()
{
    QModelIndex index = ui->tbl_group->currentIndex();
    if (index.row() > 0) {
        m_itemModel->moveUp(index);
        ui->tbl_group->selectRow(index.row() - 1);
    }
}

void DisciplineGroupDialog::moveDown()
{
    QModelIndex index = ui->tbl_group->currentIndex();
    if (index.row() < m_itemModel->rowCount() - 1) {
        m_itemModel->moveDown(index);
        ui->tbl_group->selectRow(index.row() + 1);
    }
}

void DisciplineGroupDialog::save()
{
    m_disciplineGroup->setName(ui->txt_name->text());
    m_disciplineGroup->setComment(ui->txt_comment->text());

    m_em->startTransaction();
    m_em->disciplineGroupRepository()->persist(m_disciplineGroup);
    m_itemModel->persistChanges();
    m_em->commitTransaction();
    done(1);
}
