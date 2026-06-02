#include "game.h"
#include "entity/cinderella.h"
#include "entity/unit.h"
#include "entity/projectile.h"
#include "gui/griditem.h"
#include "gui/unititem.h"
#include <QGraphicsScene>
#include <QtMath>
#include <QGraphicsRectItem>
#include <QQueue>
#include <QSet>

//角色
#include"entity/cinderella.h"



namespace {
constexpr qreal kZGrid = 0.0;
constexpr qreal kZUnit = 1.0;
constexpr qreal kZDraggingUnit = 2.0;
}

Game::Game(QObject* parent)
    : QObject(parent)
    , m_scene(new QGraphicsScene(this))
    , m_dragActive(false)
    , m_activeUnitId(-1)
    , m_sourceGrid(-1, -1)
    , m_geometry(Board::ROWS, Board::COLS, 90)//cellsize->100
    , m_combatTimer(new QTimer(this))
{
    connect(m_combatTimer,&QTimer::timeout,this,&Game::onCombatTick);
}

Game::~Game()
{
    qDeleteAll(m_units);
    m_units.clear();
}

/////////////////////////
//初始化
void Game::initialize()
{
    createStarterUnitsIfNeeded();
    buildScene();
    reset();
}

void Game::createStarterUnitsIfNeeded()
{
    //这里可以去掉，后面也理应
    if (!m_units.isEmpty()) {
        return;
    }
    Unit* unit1 = new cinderella();
    Unit* unit2 = new Unit("弓手");
    Unit* unit3 = new Unit("法师");
    unit1->setOwner(Unit::Owner::PlayerCtrl);
    unit2->setOwner(Unit::Owner::PlayerCtrl);
    unit3->setOwner(Unit::Owner::PlayerCtrl);
    m_units.append(unit1);
    m_units.append(unit2);
    m_units.append(unit3);

    //设置两个敌方模型
    Unit* enemy1 = new Unit("步兵");
    enemy1->setOwner(Unit::Owner::EnemyCtrl);
    m_units.append(enemy1);
    Unit* enemy2 = new Unit("枪兵");
    enemy2->setOwner(Unit::Owner::EnemyCtrl);
    m_units.append(enemy2);
}

void Game::buildScene()
{
    m_scene->clear();
    m_gridItems.clear();
    m_unitItems.clear();
    m_unitItemById.clear();

    QRectF totalBounds;
    bool first = true;
    for (int row = 0; row < Board::ROWS; ++row) {
        for (int col = 0; col < Board::COLS; ++col) {
            GridItem* gridItem = new GridItem(row, col, m_geometry.cellSize());
            gridItem->setZValue(kZGrid);
            gridItem->setBaseColor(row < Board::ROWS / 2 ? QColor(80, 60, 60) : QColor(60, 60, 80));
            gridItem->setPos(m_geometry.gridToWorld(row, col).x()- m_geometry.cellSize()/2.0,
                             m_geometry.gridToWorld(row, col).y() - m_geometry.cellSize() / 2.0);

            m_scene->addItem(gridItem);
            m_gridItems.push_back(gridItem);

            const QRectF bounds = gridItem->sceneBoundingRect();
            totalBounds = first ? bounds : totalBounds.united(bounds);
            first = false;

        }
    }

    for (Unit* unit : m_units) {
        UnitItem* unitItem = new UnitItem(unit);
        unitItem->setZValue(kZUnit);
        m_scene->addItem(unitItem);
        m_unitItems.push_back(unitItem);
        m_unitItemById[unit->id()] = unitItem;

        connect(unitItem, &UnitItem::dragStarted,
                this, &Game::handleDragStarted);
        connect(unitItem, &UnitItem::dragMoved,
                this, &Game::handleDragMoved);
        connect(unitItem, &UnitItem::dragDropped,
                this, &Game::handleDropCommand);
    }

    for (int i=0;i<Bench::Slots;i++){
        QPointF center = m_geometry.benchSlotToWorld(i, Bench::Slots);
        QGraphicsRectItem* slot = new QGraphicsRectItem{
            center.x() - 35,// 左上角x（70/2=35）
            center.y() - 40,// 左上角y（80/2=40）
            70,// 宽
            80// 高
        };
        slot->setPen(QPen(QColor(80, 80, 80), 2));
        // 边框：深灰 2px
        slot->setBrush(QColor(50, 50, 50));
        // 填充：深灰
        slot->setZValue(kZGrid);
        // z=0，和棋盘格同层
        m_scene->addItem(slot);
    }

    QRectF benchBounds(
        m_geometry.benchSlotToWorld(0, Bench::Slots).x() - 35,
        m_geometry.benchSlotToWorld(0, Bench::Slots).y() - 40,
        Bench::Slots * 70.0,
        80
        );
    totalBounds = totalBounds.united(benchBounds);

    m_scene->setSceneRect(totalBounds.adjusted(0,0, 0, 0));
}

void Game::reset()
{
    m_board.clear();
    m_bench.clear();

    m_combatTimer->stop();
    m_phase =Phase::Prep;
    m_player.setPopulationUsed(0);

    for(Unit* unit : m_units){
        if(!unit)continue;
        unit->resetToDefault();//回复到初始状态
        unit->clearProjectiles();
        if(unit->owner() == Unit::Owner::PlayerCtrl){
            bool placed = false;
            for(int i=0; i<Bench::Slots;i++){
                if(m_bench.isSlotEmpty(i)){
                    m_bench.addUnit(unit,i);
                    placed = true;
                    break;
                }
            }
            if(placed)continue;
            for(int row=Board::ROWS - 1;row>=Board::ROWS/2;--row){
                for(int col =0 ; col <Board::COLS;++col){
                    if(!m_board.hasUnitAt(QPoint(col,row))){
                        m_board.addUnit(unit,QPoint(col,row));
                        placed = true;
                        m_player.changePopulationUsed(1);
                        break;
                    }
                    if(placed)break;
                }
                if(placed)break;
            }
        }else{
            for(int row=0;row<Board::ROWS/2;++row){
                bool placed = false;
                for(int col =0 ; col <Board::COLS;++col){
                    if(!m_board.hasUnitAt(QPoint(col,row))){
                        m_board.addUnit(unit,QPoint(col,row));
                        placed = true;
                        break;
                    }
                }
                if(placed)break;
            }
        }
    }
    syncUnits();
    emit phaseChanged();
    emit playerInfoChanged();
}

/////////////////////////
//处理拖动操作
void Game::handleDragStarted(int unitId, const QPoint& sourceGrid, const QPointF&)
{
    Unit* unit = findUnitById(unitId);
    if(!unit || unit->owner() !=Unit::Owner::PlayerCtrl){
        return;
    }

    m_dragActive = true;
    m_activeUnitId = unitId;
    m_sourceGrid = sourceGrid;

    UnitItem* item = findUnitItem(unitId);
    if (item) {
        item->setZValue(kZDraggingUnit);
    }
}

void Game::handleDragMoved(int unitId, const QPoint&, const QPointF& scenePos)
{
    if (!m_dragActive) {
        return;
    }

    clearGridHighlights();

    int benchSlot = m_geometry.benchSlotAt(scenePos, Bench::Slots);
    QPoint target;
    if (benchSlot >= 0) {
        target = QPoint(benchSlot, -1);   // Bench 坐标
    } else {
        target = m_geometry.worldToGrid(scenePos);   // 棋盘坐标
    }

    GridItem* targetItem = findGridItem(target);
    if (!targetItem) {
        return;
    }

    targetItem->setHoverActive(true);

    if (canApplyDrop(unitId, m_sourceGrid, target)) {
        targetItem->setDropActive(true);
    }
}

void Game::handleDropCommand(int unitId, const QPoint& sourceGrid, const QPointF& scenePos)
{
    if (!m_dragActive) {
        return;
    }

    int benchSlot = m_geometry.benchSlotAt(scenePos, Bench::Slots);
    QPoint target;
    if (benchSlot >= 0)
        target = QPoint(benchSlot, -1);
    else
        target = m_geometry.worldToGrid(scenePos);

    clearGridHighlights();
    if (canApplyDrop(unitId, sourceGrid, target)) {
        applyDrop(unitId, target);
    }

    UnitItem* item = findUnitItem(m_activeUnitId);
    if (item) {
        item->setZValue(kZUnit);
    }

    m_dragActive = false;
    m_activeUnitId = -1;
    m_sourceGrid = QPoint(-1, -1);

    syncUnits();
}

///////////////////////
//查找操作
Unit* Game::findUnitById(int unitId) const
{
    for (Unit* unit : m_units) {
        if (unit && unit->id() == unitId) {
            return unit;
        }
    }
    return nullptr;
}

GridItem* Game::findGridItem(const QPoint& gridPos) const
{
    for (GridItem* item : m_gridItems) {
        if (item && item->gridPos() == gridPos) {
            return item;
        }
    }
    return nullptr;
}

UnitItem* Game::findUnitItem(int unitId) const
{
    auto it = m_unitItemById.find(unitId);
    if (it == m_unitItemById.end()) {
        return nullptr;
    }
    return it->second;
}

///////////////////////
//
void Game::clearGridHighlights()
{
    for (GridItem* item : m_gridItems) {
        if (!item) {
            continue;
        }
        item->setHoverActive(false);
        item->setDropActive(false);
    }
}

bool Game::canApplyDrop(int unitId, const QPoint& source, const QPoint& target) const
{
    //当前阶段
    if(m_phase != Phase::Prep)return false;

    Unit* unit = findUnitById(unitId);
    if (!unit) {
        return false;
    }

    bool sourceOnBench = (source.y() == -1);
    bool targetOnBench = (target.y() == -1);

    //交换位置逻辑
    if(target == source)return false;
    Unit* target_unit;
    if(!targetOnBench)target_unit = m_board.getUnitAt(target);
    if(targetOnBench)target_unit = m_bench.getUnitAt(target.x());
    if(target_unit){
        if(target_unit->owner() == Unit::Owner::PlayerCtrl){
            return true;
        }
    }

    //上面和下面有重复，但无害
    //board ->board
    if (!sourceOnBench && !targetOnBench) {
        if (!m_board.isValidPosition(source) || !m_board.isValidPosition(target)) {
            return false;
        }
        if (!m_board.isPlayerHalf(source) || !m_board.isPlayerHalf(target)) {
            return false;
        }
        if (source == target || m_board.hasUnitAt(target)) {
            return false;
        }
        return m_board.getUnitAt(source) == unit;
    }
    //board -> bench
    if (!sourceOnBench && targetOnBench) {
        int slot = target.x();
        return slot >= 0 && slot < Bench::Slots && m_bench.isSlotEmpty(slot);
    }
    //bench -> board
    if (sourceOnBench && !targetOnBench) {
        if (!m_player.canDeploy()) return false;
        if (!m_board.isValidPosition(target)) return false;
        if (!m_board.isPlayerHalf(target)) return false;
        if (m_board.hasUnitAt(target)) return false;
        return true;
    }
    //bench -> bench
    if (sourceOnBench && targetOnBench) {
        int slot = target.x();
        return slot >= 0 && slot < Bench::Slots && m_bench.isSlotEmpty(slot);
    }
    //我还想设计一个交换逻辑

    return false;
}

void Game::applyDrop(int unitId, const QPoint& target)
{
    Unit* unit = findUnitById(unitId);
    if (!unit) {
        return;
    }

    bool sourceOnBench = (m_sourceGrid.y() == -1);
    bool targetOnBench = (target.y() == -1);

    //交换逻辑
    Unit* target_unit;
    if(!targetOnBench)target_unit = m_board.getUnitAt(target);
    if(targetOnBench)target_unit = m_bench.getUnitAt(target.x());
    if(target_unit){
        QPoint unit_point = unit->position();
        if (!sourceOnBench && !targetOnBench) {
            // 棋盘 → 棋盘
            m_board.removeUnit(unit);
            m_board.removeUnit(target_unit);
            m_board.addUnit(unit, target);
            m_board.addUnit(target_unit,unit_point);
        } else if (!sourceOnBench && targetOnBench) {
            // 棋盘 → Bench
            m_board.removeUnit(unit);
            m_bench.removeUnit(target_unit);
            m_bench.addUnit(unit, target.x());
            m_board.addUnit(target_unit,unit_point);
        }else if (sourceOnBench && !targetOnBench) {
            // Bench → 棋盘
            m_bench.removeUnit(unit);
            m_board.removeUnit(target_unit);
            m_bench.addUnit(target_unit,unit_point.x());
            m_board.addUnit(unit, target);
        }else if(sourceOnBench &&targetOnBench){
            // Bench → Bench（换槽）
            m_bench.removeUnit(unit);
            m_bench.removeUnit(target_unit);
            m_bench.addUnit(unit, target.x());
            m_bench.addUnit(target_unit,unit_point.x());
        }
        emit playerInfoChanged();
        return;
    }

    if (!sourceOnBench && !targetOnBench) {
        // 棋盘 → 棋盘
        m_board.removeUnit(unit);
        m_board.addUnit(unit, target);
    } else if (!sourceOnBench && targetOnBench) {
        // 棋盘 → Bench
        m_board.removeUnit(unit);
        m_bench.addUnit(unit, target.x());
        m_player.changePopulationUsed(-1);
    }else if (sourceOnBench && !targetOnBench) {
        // Bench → 棋盘
        m_bench.removeUnit(unit);
        m_board.addUnit(unit, target);
        m_player.changePopulationUsed(1);
    }else if(sourceOnBench &&targetOnBench){
        // Bench → Bench（换槽）
        m_bench.removeUnit(unit);
        m_bench.addUnit(unit, target.x());
    }
     emit playerInfoChanged();
}

void Game::syncUnits()
{
    clearGridHighlights();

    for (UnitItem* item : m_unitItems) {
        if (!item || !item->unit()) {
            continue;
        }

        const QPoint pos = item->unit()->position();
        // 情况1：在 Bench 上
        if (pos.y() == -1) {
            int slot = pos.x();
            if (slot >= 0 && slot < Bench::Slots && m_bench.getUnitAt(slot) ==item->unit()) {
                item->setVisible(true);
                item->setGridPos(pos);
                item->setPos(m_geometry.benchSlotToWorld(slot, Bench::Slots));
                item->setZValue(kZUnit);
                item->update();
            } else {
                item->setVisible(false);
            }
            continue;
        }

        // 情况2：在棋盘上
        if (!m_board.isValidPosition(pos) || m_board.getUnitAt(pos) != item->unit()) {
            item->setVisible(false);
            continue;
        }

        item->setVisible(true);
        item->setGridPos(pos);
        item->setPos(m_geometry.gridToWorld(pos.y(), pos.x()));
        item->setZValue(kZUnit);
        item->update();
    }
}

////////////////////////
//Timer，进入战斗模式启用
void Game::startCombat(){
    m_phase = Phase::Combat;
    m_combatFrame = 0 ;
    m_combatTimer->start(16);
    emit phaseChanged();
}

void Game::onCombatTick(){
    if(m_phase != Phase::Combat)return;
    m_combatFrame++;

    for(Unit* unit :m_units){
        if(!unit || unit->isDead()) continue;
        if(unit->position().y() == -1)continue;

        unit->updateTarget(m_units);
        unit->decideState();
        unit->tickCooldowns(this);

        if (unit->state() == Unit::State::Moving && unit->moveCooldown() <= 0) {
            executeMovement(unit);
        }
        if (unit->state() == Unit::State::Attacking &&unit->attackCooldown() <= 0) {
            executeAttack(unit);
        }
    }

    // 更新所有投射物（不管射手死活，子弹已经飞出去了）
    for (Unit* unit : m_units) {
        if (!unit) continue;//原来这里角色死了就continue会导致其发射出去的子弹不再移动，一直显示的Bug
        unit->updateProjectiles();
    }

    // 移除死亡单位（飞弹到达致死的）
    for (Unit* unit : m_units) {
        if (!unit) continue;
        if (unit->state() == Unit::State::Dead && unit->position().y() != -1) {
            m_board.removeUnit(unit);
        }
    }

    syncUnits();
    checkCombatEnd();
}

void Game::executeMovement(Unit* unit)
{
    Unit* target = unit->target();
    if (!target) return;

    QVector<QPoint>& path = unit->path();

    path = findpath(unit->position(), target->position(), unit);
    if (path.isEmpty()) {
        unit->setState(Unit::State::Idle);
        return;
    }

    QPoint nextPos = path.takeFirst();

    m_board.removeUnit(unit);
    m_board.addUnit(unit, nextPos);

    unit->setMoveCooldown(unit->moveSpeed());

    if (unit->isInRangeOf(target)) {
        unit->setState(Unit::State::Attacking);
        unit->setAttackCooldown(unit->attackSpeed());
    }
}

void Game::executeAttack(Unit* unit) {
    Unit* target = unit->target();
    if (!target) return;

    // 创建攻击特效 → 返回 nullptr 表示近战（直接扣血），否则飞弹
    Projectile* proj = unit->createProjectile(target, &m_geometry);

    if (proj) {
        // 远程：飞弹到达后才扣血
        unit->addProjectile(proj);
        m_scene->addItem(proj->item());
        QPoint attackerPos = unit->position();
        proj->setStartPos(m_geometry.gridToWorld(attackerPos.y(), attackerPos.x()));
    } else {
        // 近战：直接扣血
        target->setHp(target->hp() - unit->atk());
    }

    // 回蓝
    unit->setMana(unit->mana() + unit->manaRecoveryAmount());

    // 冷却
    unit->setAttackCooldown(unit->attackSpeed());

    // 目标死亡清理
    if (target->isDead()) {
        m_board.removeUnit(target);
        target->setState(Unit::State::Dead);
        unit->settarget(nullptr);
        unit->setState(Unit::State::Idle);
    }
}
////////////////////////
//寻路
QVector<QPoint> Game::findpath(const QPoint& start, const QPoint& end,Unit* self)
{
    //这里的end会传目标的位置，要注意不能两个占在同一个格子上了
    if(start == end){return {};}
    const QPoint dirs[] = {
        QPoint(0,-1),//上
        QPoint(0, 1),//下
        QPoint(-1,0),//左
        QPoint(1, 0) //右
    };
    QQueue<QPoint> queue;
    QHash<QPoint, QPoint> cameFrom;
    QSet<QPoint> visited;

    queue.enqueue(start);
    visited.insert(start);

    while(!queue.isEmpty()){
        QPoint cur = queue.dequeue();
        for (const QPoint& dir : dirs){
            QPoint next(cur.x() + dir.x() , cur.y() + dir.y());
            if(!m_board.isValidPosition(next))continue;
            if(visited.contains(next))continue;
            if (next == end) {
                cameFrom[next] = cur;
                QVector<QPoint> path;
                QPoint step = cur;//不能两个角色占在同一个格子
                while (step != start) {
                    path.prepend(step);
                    step = cameFrom[step];
                }
                return path;
            }
            Unit* occupant = m_board.getUnitAt(next);
            if(occupant && occupant!=self)continue;//排除格子上有人或者回到起点
            cameFrom[next] = cur;
            visited.insert(next);
            queue.enqueue(next);
        }
    }

    return {};
}

/////////////////////////////////////////////////////////////////////////
//结算阶段
void Game::checkCombatEnd()
{
    if (m_phase != Phase::Combat) return;

    bool hasPlayer = false;
    bool hasEnemy = false;

    for (Unit* unit : m_units) {
        if (!unit || unit->isDead()) continue;
        if (unit->position().y() == -1) continue; // bench 单位不参战

        if (unit->owner() == Unit::Owner::PlayerCtrl)
            hasPlayer = true;
        else
            hasEnemy = true;

        if (hasPlayer && hasEnemy) return; // 双方都还有人
    }

    m_combatTimer->stop();
    m_phase = Phase::Resolve;

    // 清除所有飞行中的子弹
    for (Unit* unit : m_units) {
        if (unit) unit->clearProjectiles();
    }

    if (!hasPlayer) {
        //现在是默认平局也扣血
        int damage = 2 + m_round;
        m_player.setHp(m_player.Hp() - damage);
        emit combatEnded(false, damage);
    } else {
        //金币奖励逻辑，可以后面封装起来，考虑更多变量（同理扣血逻辑）
        int goldReward = 5 + m_round;
        m_player.changeGold(goldReward);
        emit combatEnded(true, goldReward);
    }

    emit phaseChanged();
    emit playerInfoChanged();
}

void Game::nextRound()
{
    // 清除所有敌方单位及其 UnitItem
    for (int i = m_units.size() - 1; i >= 0; --i) {
        //倒着删时，被删元素后面的索引变化不影响还没遍历到的前面的元素。

        if (m_units[i] && m_units[i]->owner() == Unit::Owner::EnemyCtrl) {
            int uid = m_units[i]->id();
            // 清理对应的 UnitItem

            // 1) 查这个敌方有没有对应的UnitItem（场景中的图形项）
            auto it = m_unitItemById.find(uid);
            if (it != m_unitItemById.end()) {

                // 2) 从 QGraphicsScene中移除（否则绘制时引用野指针崩溃）
                m_scene->removeItem(it->second);

                // 3) 从 m_unitItems 向量中删掉
                //    std::remove 把匹配元素移到末尾 →erase 真正删除
                m_unitItems.erase(std::remove(m_unitItems.begin(), m_unitItems.end(), it->second), m_unitItems.end());

                // 4) 释放 UnitItem 内存
                delete it->second;

                // 5) 从 id→UnitItem 映射表删掉
                m_unitItemById.erase(it);
            }

            // 6) 释放 Unit 对象（逻辑数据）
            delete m_units[i];

            // 7) 从 m_units 列表删掉
            m_units.removeAt(i);
        }
    }

    // 清除场上残留子弹
    for (Unit* unit : m_units) {
        if (unit) unit->clearProjectiles();
    }

    m_round++;
    spawnEnemiesForRound(m_round);

    // 为新生成的敌方创建 UnitItem
    for (Unit* unit : m_units) {
        if (m_unitItemById.find(unit->id()) == m_unitItemById.end()) {
            // find() == end() 表示这个 Unit 在映射表里不存在
            // 也就是说它是刚刚 spawnEnemiesForRound() 新建的

            // 1) 创建 UnitItem，传入 Unit* 建立关联
            UnitItem* unitItem = new UnitItem(unit);
            unitItem->setZValue(kZUnit);
            // 2) 注册到 scene（不调用 addItem 就不会显示）
            m_scene->addItem(unitItem);
            // 3) 注册到 m_unitItems（syncUnits需要遍历它来更新位置/可见性）
            m_unitItems.push_back(unitItem);
            // 4) 建立 id → UnitItem 的快速查找
            m_unitItemById[unit->id()] = unitItem;
            // 5) 连接拖拽信号（敌方不需要拖拽，但 connect无害，实测拖不动，应该是有逻辑检查）
            connect(unitItem, &UnitItem::dragStarted, this, &Game::handleDragStarted);
            connect(unitItem, &UnitItem::dragMoved, this, &Game::handleDragMoved);
            connect(unitItem, &UnitItem::dragDropped, this, &Game::handleDropCommand);
        }
    }

    reset();
    emit phaseChanged();
}

void Game::spawnEnemiesForRound(int round)
{
    int count = qMin(1 + round, 8); // 每轮多一个，最多 8 个
    for (int i = 0; i < count; ++i) {
        Unit* enemy;
        if (i % 3 == 0)
            enemy = new Unit("步兵");
        else if (i % 3 == 1)
            enemy = new Unit("枪兵");
        else
            enemy = new Unit("弓手");

        enemy->setOwner(Unit::Owner::EnemyCtrl);

        // 随轮次成长
        enemy->setMaxHp(300 + round * 50);
        enemy->setHp(enemy->maxHp());
        enemy->setAtk(10 + round * 3);

        m_units.append(enemy);
    }
}


























