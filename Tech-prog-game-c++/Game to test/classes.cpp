#include "classes.h"
#include <cmath>
#include <cstdlib>
#include <queue>
#include <iostream>
#include <algorithm>

CUnit::CUnit(int health, int damage, fraction fraction, warriorType warriorType): health_(health),
             damage_(damage), fraction_(fraction), type_(warriorType) {}

int CUnit::getHealth() const {
    return health_;
}

const int CUnit::getDamage() const {
    return damage_;
}

void CUnit::reduceHealth(int loss) {
    health_ -= loss;
}

bool CUnit::isDead() const {
    return health_ <= 0;
}

warriorType CUnit::getWarriorType() const {
    return type_;
}

fraction CUnit::getFraction() const {
    return fraction_;
}

CUnit::CUnit(const CUnit& out) {
    this->damage_ = out.damage_;
    this->fraction_ = out.fraction_;
    this->health_ = out.health_;
    this->type_ = out.type_;
}

CUnit& CUnit::operator= (const CUnit& out) {
    if (this == &out) {
        return *this;
    }
    this->damage_= out.damage_;
    this->type_ = out.type_;
    this->health_ = out.health_;
    this->fraction_ = out.fraction_;
    return *this;
}

bool betweenBorders(int cur_x, int cur_y, int new_x, int new_y, int leftBorder, int rightBorder) {
    return  (int)leftBorder <= (abs(cur_x - new_x) + abs(cur_y - new_y)) && (abs(cur_x - new_x) +
            abs(cur_y - new_y)) <= (int)rightBorder;
}

bool insideBattleField(int cur_x, int cur_y, std::shared_ptr<std::vector<std::vector<CUnit*> > > battleField) {
    return battleField != nullptr && cur_x >= 0 && cur_x < (int)battleField->size() && cur_y >= 0 && cur_y < (int)battleField->at(0).size();
}

CAttackingLeader::CAttackingLeader(): CUnit(6, 2, attacking, leader) {}

bool CAttackingLeader::canMove(int cur_x, int cur_y, int new_x, int new_y) const {
    return betweenBorders(cur_x, cur_y, new_x, new_y, 1, 2);
}

bool CAttackingLeader::canAttack(int cur_x, int cur_y, int new_x, int new_y) const {
    return betweenBorders(cur_x, cur_y, new_x, new_y, 1, 4);
}

CDefendingLeader::CDefendingLeader(): CUnit(1, 0, defending, leader) {}

bool CDefendingLeader::canMove(int cur_x, int cur_y, int new_x, int new_y) const {
    return abs(cur_x - new_x) + abs(cur_y - new_y) == 1;
}

bool CDefendingLeader::canAttack(int cur_x, int cur_y, int new_x, int new_y) const {
    return false; // Defending leader can't attack!
}

CAttackingInfantry::CAttackingInfantry(): CUnit(2, 2, attacking, infantry) {}

bool CAttackingInfantry::canMove(int cur_x, int cur_y, int new_x, int new_y) const {
    return betweenBorders(cur_x, cur_y, new_x, new_y, 1, 2);
}
bool CAttackingInfantry::canAttack(int cur_x, int cur_y, int new_x, int new_y) const {
    return abs(cur_x - new_x) + abs(cur_y - new_y) == 1;
}

CDefendingInfantry::CDefendingInfantry(): CUnit(1, 2, defending, infantry) {}

bool CDefendingInfantry::canMove(int cur_x, int cur_y, int new_x, int new_y) const {
    return betweenBorders(cur_x, cur_y, new_x, new_y, 1, 2);
}

bool CDefendingInfantry::canAttack(int cur_x, int cur_y, int new_x, int new_y) const {
    return abs(cur_x - new_x) + abs(cur_y - new_y) == 1;
}

CAttackingShooter::CAttackingShooter(): CUnit(1, 1, attacking, shooter) {}

bool CAttackingShooter::canMove(int cur_x, int cur_y, int new_x, int new_y) const {
    return abs(cur_x - new_x) + abs(cur_y - new_y) == 1;
}

bool CAttackingShooter::canAttack(int cur_x, int cur_y, int new_x, int new_y) const {
    return betweenBorders(cur_x, cur_y, new_x, new_y, 1, 4);
}

CDefendingShooter::CDefendingShooter(): CUnit(1, 2, defending, shooter) {}

bool CDefendingShooter::canMove(int cur_x, int cur_y, int new_x, int new_y) const {
    return abs(cur_x - new_x) + abs(cur_y - new_y) == 1;
}

bool CDefendingShooter::canAttack(int cur_x, int cur_y, int new_x, int new_y) const {
    return betweenBorders(cur_x, cur_y, new_x, new_y, 1, 4);
}

CUnit* CAttackingFactory::createLeader() const {
    return new CAttackingLeader;
}

CUnit* CAttackingFactory::createInfantry() const {
    return new CAttackingInfantry;
}

CUnit* CAttackingFactory::createShooter() const {
    return new CAttackingShooter;
}

CUnit* CDefendingFactory::createLeader() const {
    return new CDefendingLeader();
}

CUnit* CDefendingFactory::createInfantry() const {
    return new CDefendingInfantry;
}

CUnit* CDefendingFactory::createShooter() const {
    return new CDefendingShooter;
}

std::shared_ptr<std::vector<std::vector<CUnit*> > > CPlayingBoard::desk_ = 0;

std::shared_ptr<std::vector<std::vector<CUnit*> > > CPlayingBoard::board() {
    if (desk_ == nullptr) {
        desk_ = std::make_shared<std::vector<std::vector<CUnit*> > >(std::vector<std::vector<CUnit*> >(boardSize,
                std::vector<CUnit*>(boardSize, nullptr)));
    }
    return desk_;
}

CPlayingBoard::~CPlayingBoard() {
    deleteBoard();
}

bool CPlayingBoard::canMove(int cur_x, int cur_y, int new_x, int new_y) {
    return insideBattleField(cur_x, cur_y, desk_) && insideBattleField(new_x, new_y, desk_) && desk_->at(cur_x)[cur_y] != nullptr &&
    desk_->at(cur_x)[cur_y]->canMove(cur_x, cur_y, new_x, new_y) && desk_->at(new_x)[new_y] == nullptr && !(cur_x == new_x && cur_y == new_y);
}

bool CPlayingBoard::canAttack(int cur_x, int cur_y, int new_x, int new_y) {
    return insideBattleField(cur_x, cur_y, desk_) && insideBattleField(new_x, new_y, desk_) && desk_->at(cur_x)[cur_y] != nullptr  && desk_->at(new_x)[new_y] != nullptr  &&
    desk_->at(cur_x)[cur_y]->canAttack(cur_x, cur_y, new_x, new_y) && desk_->at(cur_x)[cur_y]->getFraction() != desk_->at(new_x)[new_y]->getFraction()
                             && !(cur_x == new_x && cur_y == new_y);
}

bool CPlayingBoard::canPlaceUnit(int cur_x, int cur_y) {
    return insideBattleField(cur_x, cur_y, desk_) && desk_->at(cur_x)[cur_y] == nullptr;
}

void CPlayingBoard::placeUnit(int cur_x, int cur_y, CUnit* unit) {
    desk_->at(cur_x)[cur_y] = unit;
}

void CPlayingBoard::attack(int cur_x, int cur_y, int new_x, int new_y) {
    int damage = desk_->at(cur_x)[cur_y]->getDamage();
    desk_->at(new_x)[new_y]->reduceHealth(damage);
}

void CPlayingBoard::deleteBoard() {
    for (size_t i = 0; i < desk_->size(); ++i) {
        for (size_t l = 0; l < desk_->at(i).size(); ++l) {
            if (desk_->at(i)[l] != nullptr) {
                delete desk_->at(i)[l];
            }
        }
    }
    desk_.reset();
}

CFactoryDecorator::CFactoryDecorator(CArmyFactory* factory): controlledFactory(factory) {}

CUnit* CFactoryDecorator::createLeader() const {
    std::cout << "New leader created!" << '\n';
    return controlledFactory->createLeader();
}

CUnit* CFactoryDecorator::createInfantry() const {
    std::cout << "New infantry created!" << '\n';
    return controlledFactory->createInfantry();
}

CUnit* CFactoryDecorator::createShooter() const {
    std::cout << "New shooter created!" << '\n';
    return controlledFactory->createShooter();
}

CNode::CNode(int x, int y, int depth): depth_(depth), moveOnTheIteration(false) {
    savedComponent_ = std::make_pair(x, y);
    children_ = (std::vector<std::shared_ptr<CNode> >());
}

bool operator <(const std::shared_ptr<CNode>& a, const std::shared_ptr<CNode>& b) {
    std::pair<int, int> pair1 = a->getSavedComponent();
    std::pair<int, int> pair2 = b->getSavedComponent();
    return (pair1 < pair2);
}

bool CComparator::operator ()(const std::shared_ptr<CNode>& a, const std::shared_ptr<CNode>& b) {
    return a < b;
}

std::pair<int, int> CNode::getSavedComponent() const {
    return savedComponent_;
}

bool CNode::addChild(int x, int y) {
    if (x == -1 && depth_ >= maxCompositeDepth - 1) {
        return false;
    }
    if (x != -1 && depth_ != maxCompositeDepth - 1) {
        return false;
    }
    CNode childNode = CNode(x, y, depth_ + 1);
    std::shared_ptr<CNode> childNodePtr = std::make_shared<CNode>(childNode);
    children_.push_back(childNodePtr);
    return true;
}

bool CNode::removeChild(int x, int y) {
    size_t idx = 0;
    for (idx = 0; idx < children_.size(); ++idx) {
        if (children_[idx]->savedComponent_ == std::make_pair(x, y)) {
            break;
        }
    }
    if (idx == children_.size() || !children_[idx]->children_.empty()) {
        return false;
    }
    children_.erase(children_.begin() + idx);
    return true;
}

std::shared_ptr<CNode> CNode::getNode(int x, int y) const {
    if (savedComponent_.first != -1) {
        return nullptr;
    }
    for (size_t i = 0; i < children_.size(); ++i) {
        if (children_[i]->savedComponent_ == std::make_pair(x, y)) {
            return children_[i];
        }
        std::shared_ptr<CNode> possibleAns = children_[i]->getNode(x, y);
        if (possibleAns != nullptr) {
            return possibleAns;
        }
    }
    return nullptr;
}

std::shared_ptr<CNode> CNode::getParentNode(int x, int y, std::shared_ptr<CNode> prevCNode = nullptr) const {
    if (savedComponent_.first != -1) {
        return nullptr;
    }
    for (size_t i = 0; i < children_.size(); ++i) {
        if (children_[i]->savedComponent_ == std::make_pair(x, y)) {
            return prevCNode;
        }
        std::shared_ptr<CNode> possibleAns = children_[i]->getParentNode(x, y, children_[i]);
        if (possibleAns != nullptr) {
            return possibleAns;
        }
    }
    return nullptr;
}

CComposite::CComposite(fraction fraction): fraction_(fraction) {
    topNode_ = std::make_shared<CNode>(CNode(-1, 1, 1));
    usedNumbers_.insert(1);
    std::shared_ptr<CNode> ptr = topNode_;
    int i = 2;
    while (i != maxCompositeDepth) {
        usedNumbers_.insert(i);
        ptr->addChild(-1, i);
        ptr = ptr->children_[0];
        i++;
    }
    std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board->at(i)[j] != nullptr && board->at(i)[j]->getFraction() == fraction_) {
                ptr->addChild(i, j);
            }
        }
    }
}

std::shared_ptr<CNode> CComposite::getNode(int x, int y) const {
    if (x == -1 && y == 1) {
        return topNode_;
    }
    return topNode_->getNode(x, y);
}

void CComposite::printComposite() const {
    std::queue<std::shared_ptr<CNode> > q;
    q.push(topNode_);
    while (!q.empty()) {
        std::shared_ptr<CNode> ptr = q.front();
        q.pop();
        if (ptr->savedComponent_.first == -1) {
            std::cout << "Structure № " << ptr->savedComponent_.second << " " << structureNames[ptr->depth_ - 1] << "."
                      << '\n';
            std::cout << "Children: ";
            std::sort(ptr->children_.begin(), ptr->children_.end(), CComparator());
            for (size_t i = 0; i < ptr->children_.size(); ++i) {
                q.push(ptr->children_[i]);
                std::cout << structureNames[ptr->children_[i]->depth_ - 1] << " ";
                if (ptr->children_[i]->savedComponent_.first != -1) {
                    std::cout << ptr->children_[i]->savedComponent_.first << ", ";
                }
                std::cout << ptr->children_[i]->savedComponent_.second << "; ";
            }
            std::cout << '\n';
        }
    }
}

bool CComposite::addChild(int x) {
    std::shared_ptr<CNode> ptr = getNode(-1, x);
    int num = 1;
    while (usedNumbers_.count(num) > 0) {
        num++;
    }
    usedNumbers_.insert(num);
    return ptr->addChild(-1, num);
}

std::shared_ptr<CNode> CComposite::getParentNode(int x, int y) const {
    if (topNode_->savedComponent_.first == x && topNode_->savedComponent_.second == y) {
        return nullptr;
    }
    return topNode_->getParentNode(x, y, topNode_);
}

bool CComposite::removeChild(int x, int y) {
    std::shared_ptr<CNode> ptr = getParentNode(x, y);
    if (ptr == nullptr) {
        return false;
    }
    if(ptr->removeChild(x, y)) {
        if(x == -1) {
            usedNumbers_.erase(y);
        }
        return true;
    }
    return false;
}

bool CComposite::switchChild (int x, int y, int comp_num) {
    if (getParentNode(x, y) == nullptr || x == -1) {
        return false;
    }
    removeChild(x, y);
    std::shared_ptr<CNode> futureComponent = getNode(-1, comp_num);
    if (futureComponent->depth_ != maxCompositeDepth - 1) {
        return false;
    }
    futureComponent->addChild(x, y);
    return true;
}

bool CPlayingBoard::canMoveComposite(const CComposite& composite, std::pair<int, int> nodePair, int xOffset, int yOffset) {
    std::shared_ptr<CNode> ptr = composite.getNode(nodePair.first, nodePair.second);
    if (ptr == nullptr) {
        return false;
    }
    std::set<std::pair<int, int> > pairsInNode;
    std::queue<std::shared_ptr<CNode> > q;
    q.push(ptr);
    while (!q.empty()) {
        std::shared_ptr<CNode> curPtr = q.front();
        q.pop();
        if (curPtr->getSavedComponent().first != -1) {
            pairsInNode.insert(curPtr->getSavedComponent());
        } else {
            for (size_t i = 0; i < curPtr->children_.size(); ++i) {
                q.push(curPtr->children_[i]);
            }
        }
    }
    bool canMove = true;
    for (auto elem: pairsInNode) {
        int cur_x = elem.first, cur_y = elem.second;
        int new_x = elem.first + xOffset, new_y = elem.second + yOffset;
        canMove = canMove && insideBattleField(cur_x, cur_y, desk_) && insideBattleField(new_x, new_y, desk_) &&
                desk_->at(cur_x)[cur_y] != nullptr && desk_->at(cur_x)[cur_y]->canMove(cur_x, cur_y, new_x, new_y) &&
                (desk_->at(new_x)[new_y] == nullptr || pairsInNode.count(std::make_pair(new_x, new_y)) > 0)
                && !(cur_x == new_x && cur_y == new_y);
    }
    return canMove;
}

void CPlayingBoard::moveComposite(int x, int y, int xOffset, int yOffset, CComposite composite) {
    if (canMoveComposite(composite, std::make_pair(x, y), xOffset, yOffset)) {
        std::pair<int, int> nodePair = std::make_pair(x, y);
        std::shared_ptr<CNode> ptr = composite.getNode(nodePair.first, nodePair.second);
        if (ptr == nullptr) {
            return;
        }
        std::queue<std::shared_ptr<CNode> > q;
        q.push(ptr);
        std::vector<std::pair<std::pair<int, int>, CUnit*> > unitPosition;
        std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
        while (!q.empty()) {
            std::shared_ptr<CNode> curPtr = q.front();
            if (curPtr->getSavedComponent().first != -1) {
                curPtr->moveOnTheIteration = true;
            }
            q.pop();
            if (curPtr->getSavedComponent().first != -1) {
                std::pair<int, int> curPair = curPtr->getSavedComponent();
                curPtr->savedComponent_ = std::make_pair(curPair.first + xOffset, curPair.second + yOffset);
                unitPosition.emplace_back(std::make_pair(curPtr->getSavedComponent(), board->at(curPair.first)[curPair.second]));
                board->at(curPair.first)[curPair.second] = nullptr;
            } else {
                for (size_t i = 0; i < curPtr->children_.size(); ++i) {
                    q.push(curPtr->children_[i]);
                }
            }
        }
        for (size_t i = 0; i < unitPosition.size(); ++i) {
            std::pair<int, int> curPair = unitPosition[i].first;
            board->at(curPair.first)[curPair.second] = unitPosition[i].second;
        }
    }
}

std::shared_ptr<CNode> CComposite::getTopNode() const {
    return topNode_;
}

bool CPlayingBoard::allMovedComposite(std::shared_ptr<CNode> topNode, int i) {
    std::queue<std::shared_ptr<CNode> > q;
    q.push(topNode);
    std::vector<std::pair<int, int> > unmovedUnits;
    std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
    while (!q.empty()) {
        std::shared_ptr<CNode> curPtr = q.front();
        q.pop();
        if (curPtr->getSavedComponent().first != -1) {
            if (!curPtr->moveOnTheIteration) {
                unmovedUnits.push_back(curPtr->getSavedComponent());
            }
        } else {
            for (size_t i = 0; i < curPtr->children_.size(); ++i) {
                q.push(curPtr->children_[i]);
            }
        }
    }
    if (unmovedUnits.empty()) {
        return true;
    }
    if (i > 0) {
        std::cout << "This composite's components were unmoved on the iteration:" << '\n';
        for (size_t i = 0; i < unmovedUnits.size(); ++i) {
            std::cout << unmovedUnits[i].first << " " << unmovedUnits[i].second << '\n';
        }
    }
    return false;
}

bool CPlayingBoard::allUnmovedComposite(std::shared_ptr<CNode> topNode) {
    std::queue<std::shared_ptr<CNode> > q;
    q.push(topNode);std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
    while (!q.empty()) {
        std::shared_ptr<CNode> curPtr = q.front();
        q.pop();
        if (curPtr->getSavedComponent().first != -1) {
            if (curPtr->moveOnTheIteration) {
                return false;
            }
        } else {
            for (size_t i = 0; i < curPtr->children_.size(); ++i) {
                q.push(curPtr->children_[i]);
            }
        }
    }
    return true;
}

void CComposite::startNewMove() {
    std::queue<std::shared_ptr<CNode> > q;
    q.push(getTopNode());
    while (!q.empty()) {
        std::shared_ptr<CNode> curPtr = q.front();
        q.pop();
        if (curPtr->getSavedComponent().first != -1) {
            curPtr->moveOnTheIteration = false;
        } else {
            for (size_t i = 0; i < curPtr->children_.size(); ++i) {
                q.push(curPtr->children_[i]);
            }
        }
    }
}

void CVisitor::visit(CDefendingInfantry) const {
    std::cout << "1";
}

void CVisitor::visit(CDefendingShooter) const {
    std::cout << "2";
}

void CVisitor::visit(CDefendingLeader) const {
    std::cout << "3";
}

void CVisitor::visit(CAttackingInfantry) const {
    std::cout << "7";
}

void CVisitor::visit(CAttackingShooter) const {
    std::cout << "8";
}

void CVisitor::visit(CAttackingLeader) const {
    std::cout << "9";
}

void CAttackingLeader::visit(CVisitor visitor) const {
    visitor.visit(*this);
}

void CAttackingInfantry::visit(CVisitor visitor) const {
    visitor.visit(*this);
}

void CAttackingShooter::visit(CVisitor visitor) const {
    visitor.visit(*this);
}

void CDefendingLeader::visit(CVisitor visitor) const {
    visitor.visit(*this);
}

void CDefendingInfantry::visit(CVisitor visitor) const {
    visitor.visit(*this);
}

void CDefendingShooter::visit(CVisitor visitor) const {
    visitor.visit(*this);
}

void CPlayingBoard::printBoard() {
    std::cout << "Current board:" << '\n';
    CVisitor visitor = CVisitor();
    for (size_t i = 0; i < desk_->size(); ++i) {
        for (size_t l = 0; l < desk_->at(i).size(); ++l) {
            if (desk_->at(i)[l] == nullptr) {
                std::cout << "x";
            } else {
                desk_->at(i)[l]->visit(visitor);
            }
            std::cout << " ";
        }
        std::cout << '\n';
    }
    std::cout << '\n' << "x - empty" << '\n';
    std::cout << "1 - defending infantry" << '\n';
    std::cout << "2 - defending shooter" << '\n';
    std::cout << "3 - defending leader" << '\n';
    std::cout << "7 - attacking infantry" << '\n';
    std::cout << "8 - attacking shooter" << '\n';
    std::cout << "9 - attacking leader" << '\n' << '\n';
}

CGame::CGame(): gameFinished(false), winner(attacking), attackingComposite(attacking), defendingComposite(defending) {
    attackingFactory = new CAttackingFactory();
    defendingFactory = new CDefendingFactory();
    CPlayingBoard::board(); // to generate the board;
    std::cout << "Welcome to the game." << '\n' << '\n';
    CPlayingBoard::printBoard();
    placeArmy();
    attackingComposite = CComposite(attacking); // to make composites correct
    defendingComposite = CComposite(defending); // to make composites correct
}


CGame::~CGame() {
    CPlayingBoard::deleteBoard();
    delete attackingFactory;
    delete defendingFactory;
}

std::pair<int, int> CGame::tryPlaceUnit() const {
    int x, y;
    std::cin >> x >> y;
    while (!CPlayingBoard::canPlaceUnit(x - 1, y - 1)) {
        std::cout << "This coordinates are unavailable, try again!" << '\n';
        std::cin >> x >> y;
    }
    return std::make_pair(x, y);
}

void CGame::placeLeader(fraction fraction) {
    std::cout << (fraction == attacking ? "Attacking" : "Defending")  << " player, please enter the coordinates of the "
                                                                         "position of your leader, separated by whitespace, "
                                                                         "correct coordinates are between 1 and " << boardSize
                                                                         << "." << '\n';
    std::pair<int, int> correctPosition = tryPlaceUnit();
    int x = correctPosition.first, y = correctPosition.second;
    CUnit* leader = (fraction == attacking ? attackingFactory->createLeader() : defendingFactory->createLeader());
    CPlayingBoard::placeUnit(x - 1, y - 1, leader);
    CPlayingBoard::printBoard();
    placeUnit(fraction, (fraction == attacking ? attackingUnits : defendingUnits));
}

void CGame::placeUnit(fraction fraction, int unitsLeft) {
    if (unitsLeft == 0 and fraction == defending) {
        return;
    } else if (unitsLeft == 0 and fraction == attacking) {
        placeLeader(defending);
        return;
    }
    std::cout << "You can place infantry or shooter: enter 1 to place infantry and 2 to place shooter." << '\n';
    int warriorType;
    std::cin >> warriorType;
    while (warriorType != 1 and warriorType != 2) {
        std::cout << "This type is unavailable, try again." << '\n';
        std::cin >> warriorType;
    }
    std::cout << (fraction == attacking ? "Attacking" : "Defending")  << " player, please enter the coordinates of the "
                                                                         "position you want to place the unit, separated "
                                                                         "by whitespace, correct coordinates are between 1 "
                                                                         "and " << boardSize << "." << '\n';
    std::cout << "The first coordinate is vertical, the second - horizontal." << '\n';
    std::pair<int, int> correct_position = tryPlaceUnit();
    int x = correct_position.first, y = correct_position.second;
    if (fraction == attacking) {
        if (warriorType == 1) {
            CUnit* warrior = attackingFactory->createInfantry();
            CPlayingBoard::placeUnit(x - 1, y - 1, warrior);
        } else {
            CUnit* warrior = attackingFactory->createShooter();
            CPlayingBoard::placeUnit(x - 1, y - 1, warrior);
        }
    } else {
        if (warriorType == 1) {
            CUnit* warrior = defendingFactory->createInfantry();
            CPlayingBoard::placeUnit(x - 1, y - 1, warrior);
        } else {
            CUnit* warrior = defendingFactory->createShooter();
            CPlayingBoard::placeUnit(x - 1, y - 1, warrior);
        }
    }
    CPlayingBoard::printBoard();
    placeUnit(fraction, unitsLeft - 1);
}

void CGame::placeArmy() {
    placeLeader(attacking);
}

void CGame::makeMove(fraction fraction) {
    CComposite composite = (fraction == attacking ? attackingComposite : defendingComposite);
    std::cout << (fraction == attacking ? "Attacking " : "Defending ") << "player move." << '\n' << '\n';
    CPlayingBoard::printBoard();
    std::cout << "Your army composite." << '\n';
    composite.printComposite();
    std::cout << '\n';
    std::cout << "Enter the composite coordinates of the unit. If you want to move the structure, the first coordinate "
                 "should be -1 and the second is the number of the structure." << '\n';
    int x, y;
    std::cin >> x >> y;
    while (!(composite.getNode(x, y) != nullptr && CPlayingBoard::allUnmovedComposite(composite.getNode(x, y)))) {
        std::cout << "This coordinates are unavailable, try again!" << '\n';
        std::cin >> x >> y;
    }
    int offsetX, offsetY;
    std::cout << "Enter the offset for the composite's component you've entered previously, separated with the whitespace."
                 "The first coordinate is vertical offset, the second - horizontal." << '\n';

    std::cin >> offsetX >> offsetY;
    while (!CPlayingBoard::canMoveComposite(composite, std::make_pair(x, y), offsetX, offsetY)) {
        std::cout << "The offset is incorrect." << '\n';
        std::cin >> offsetX >> offsetY;
    }
    CPlayingBoard::moveComposite(x, y, offsetX, offsetY, composite);
}

void CGame::makeMovePhase(fraction fraction) {
    if (gameFinished) {
        return;
    }
    CComposite composite = (fraction == attacking ? attackingComposite : defendingComposite);
    while (!CPlayingBoard::allMovedComposite(composite.getTopNode(), 0)) {
        makeMove(fraction);
    }
    if (fraction == attacking) {
        composite.startNewMove();
        makeMovePhase(defending);
    } else {
        composite.startNewMove();
        makeAttackPhase(attacking);
    }
}

bool CPlayingBoard::canAttack(int x, int y) {
    for (size_t i = 0; i < desk_->size(); ++i) {
        for (size_t l = 0; l < desk_->at(i).size(); ++l) {
            if (CPlayingBoard::canAttack(x, y, i, l)) {
                return true;
            }
        }
    }
    return false;
}

size_t CComposite::size() const {
    size_t size = 0;
    std::queue<std::shared_ptr<CNode> > q;
    q.push(topNode_);std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
    while (!q.empty()) {
        std::shared_ptr<CNode> curPtr = q.front();
        q.pop();
        if (curPtr->getSavedComponent().first == -1) {
            for (size_t i = 0; i < curPtr->children_.size(); ++i) {
                q.push(curPtr->children_[i]);
            }
        } else {
            size++;
        }
    }
    return size;
}

void CGame::makeAttack(fraction fraction) {
    CComposite composite = (fraction == attacking ? attackingComposite : defendingComposite);
    CComposite enemyComposite = (fraction == attacking ? defendingComposite : attackingComposite);
    for (size_t i = 0; i < CPlayingBoard::desk_->size() && !gameFinished; ++i) {
        for (size_t l = 0; l < CPlayingBoard::desk_->at(i).size() && !gameFinished; ++l) {
            if (CPlayingBoard::canAttack(i, l) && CPlayingBoard::desk_->at(i)[l]->getFraction() == fraction) {
                CPlayingBoard::printBoard();
                std::cout << "Current attacking unit's position " << i + 1 << " " << l + 1 << "." << '\n';
                std::cout << "Write the coordinates of unit you want to attack, the coordinates must be separated with "
                             "the whitespace. Coordinates must be between 1 and " << boardSize << "." << '\n';
                std::cout << "The first coordinate is horizontal, the second - vertical." << '\n';
                int x, y;
                std::cin >> x >> y;
                while (!CPlayingBoard::canAttack(i, l, x - 1, y - 1)) {
                    std::cout << "This coordinates are unavailable, maybe unit can't reach the target, try again!" << '\n';
                    std::cin >> x >> y;
                }
                CPlayingBoard::attack(i, l, x - 1, y - 1);
                if (CPlayingBoard::desk_->at(x - 1)[y - 1]->getHealth() <= 0) {
                    if (CPlayingBoard::desk_->at(x - 1)[y - 1]->getWarriorType() == leader &&
                    CPlayingBoard::desk_->at(x - 1)[y - 1]->getFraction() == defending) {
                        gameFinished = true;
                        winner = fraction;
                    }
                    delete CPlayingBoard::desk_->at(x - 1)[y - 1];
                    CPlayingBoard::desk_->at(x - 1)[y - 1] = nullptr;
                    std::shared_ptr<CNode> parent = enemyComposite.getParentNode(x - 1, y - 1);
                    parent->removeChild(x - 1, y - 1);
                    if (enemyComposite.size() == 0) {
                        gameFinished = true;
                        winner = fraction;
                    }
                }
            }
        }
    }
}

void CGame::makeAttackPhase(fraction fraction) {
    CComposite composite = (fraction == attacking ? attackingComposite : defendingComposite);
    makeAttack(fraction);
    if (gameFinished) {
        return;
    }
    if (fraction == attacking) {
        composite.startNewMove();
        makeAttackPhase(defending);
    } else {
        composite.startNewMove();
        makeEditCompositePhase(attacking);
    }
}

void CGame::makeEditComposite(fraction fraction) {
    CComposite composite = (fraction == attacking ? attackingComposite : defendingComposite);
    std::cout << (fraction == attacking ? "Attacking " : "Defending ") << "player can change composite." << '\n';
    std::cout << '\n' << "Composite" << '\n';
    composite.printComposite();
    std::cout << '\n';
    std::cout << "Enter 1 if you want to add a new component, 2 if you want to change soldier's parent, 3 if you want "
                 "to exit the stage."<< '\n';
    int state;
    std::cin >> state;
    while (state != 1 && state != 2 && state != 3) {
        std::cout << "Incorrect command entered." << '\n';
    }
    while (state != 3) {
        if (state == 1) {
            std::cout << "Enter the number of the future parent structure." << '\n';
            int structureNumber;
            std::cin >> structureNumber;
            while (composite.getNode(-1, structureNumber) == nullptr) {
                std::cout << "Incorrect number entered" << '\n';
                std::cin >> structureNumber;
            }
            composite.addChild(structureNumber);
        } else if (state == 2) {
            std::cout << "Enter the number of you structure." << '\n';
            int structureNumber;
            std::cin >> structureNumber;
            while (composite.getNode(-1, structureNumber) == nullptr) {
                std::cout << "Incorrect number entered" << '\n';
                std::cin >> structureNumber;
            }
            std::cout << "Enter the number of the soldier you want to change the parent of: two numbers separated with "
                         "the whitespace." << '\n';
            int x, y;
            std::cin >> x >> y;
            while (x == -1 || composite.getNode(x, y) == nullptr) {
                std::cout << "Incorrect data entered, try again!" << '\n';
                std::cin >> x >> y;
            }
            composite.switchChild(x, y, structureNumber);
        }
        composite.printComposite();
        std::cout << '\n';
        std::cout << "Enter 1 if you want to add a new component, 2 if you want to change soldier's parent, 3 if you want "
                     "to exit the stage."<< '\n';
        std::cin >> state;
        while (state != 1 && state != 2 && state != 3) {
            std::cout << "Incorrect command entered." << '\n';
        }
    }
}

void CGame::makeEditCompositePhase(fraction fraction) {
    CComposite composite = (fraction == attacking ? attackingComposite : defendingComposite);
    makeEditComposite(fraction);
    if (gameFinished) {
        return;
    }
    if (fraction == attacking) {
        composite.startNewMove();
        makeEditCompositePhase(defending);
    } else {
        composite.startNewMove();
        makeMovePhase(attacking);
    }
}

void CGame::game() {
    makeEditCompositePhase(attacking);
    std::cout << "Game over!" << '\n';
    std::cout << (winner == attacking ? "Attacking " : "Defending ") << "team won!" << '\n';
}