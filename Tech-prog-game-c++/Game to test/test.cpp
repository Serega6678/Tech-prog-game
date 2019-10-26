#include "classes.cpp"
#include <gtest/gtest.h>
#include <utility>

TEST(Correct_factory, defending_units) {
    CDefendingFactory defendingFactory = CDefendingFactory();
    CUnit* leaderUnit = defendingFactory.createLeader();
    CUnit* infantryUnit = defendingFactory.createInfantry();
    CUnit* shooterUnit = defendingFactory.createShooter();
    ASSERT_TRUE(leaderUnit->getFraction() == defending && leaderUnit->getWarriorType() == leader);
    ASSERT_TRUE(infantryUnit->getFraction() == defending && infantryUnit->getWarriorType() == infantry);
    ASSERT_TRUE(shooterUnit->getFraction() == defending && shooterUnit->getWarriorType() == shooter);
    delete leaderUnit;
    delete infantryUnit;
    delete shooterUnit;
}

TEST(Correct_factory, attacking_units) {
    CAttackingFactory attackingFactory = CAttackingFactory();
    CUnit* leaderUnit = attackingFactory.createLeader();
    CUnit* infantryUnit = attackingFactory.createInfantry();
    CUnit* shooterUnit = attackingFactory.createShooter();
    ASSERT_TRUE(leaderUnit->getFraction() == attacking && leaderUnit->getWarriorType() == leader);
    ASSERT_TRUE(infantryUnit->getFraction() == attacking && infantryUnit->getWarriorType() == infantry);
    ASSERT_TRUE(shooterUnit->getFraction() == attacking && shooterUnit->getWarriorType() == shooter);
    delete leaderUnit;
    delete infantryUnit;
    delete shooterUnit;
}

TEST(Correct_factory, decorator) {
    CAttackingFactory* attackingFactory = new CAttackingFactory();
    CFactoryDecorator factory = CFactoryDecorator(attackingFactory);
    CUnit* shooterUnit = factory.createShooter();
    ASSERT_TRUE(shooterUnit->getFraction() == attacking && shooterUnit->getWarriorType() == shooter);
    delete shooterUnit; // юнит тот, в консоли тестирования cout появился
}

TEST(Correct_board, creation) {
    std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
    ASSERT_TRUE(board->size() == boardSize);
    for (size_t i = 0; i < boardSize; ++i) {
        ASSERT_TRUE(board->at(i).size() == boardSize);
        for(size_t l = 0; l < boardSize; ++l) {
            ASSERT_TRUE(board->at(i)[l] == nullptr);
        }
    }
    board.reset();
    CPlayingBoard::deleteBoard();
}

TEST(Correct_board, singleton) {
    std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
    CAttackingFactory attackingFactory = CAttackingFactory();
    CUnit* leader = attackingFactory.createLeader();
    board->at(0)[0] = leader;
    std::shared_ptr<std::vector<std::vector<CUnit*> > > newBoard = CPlayingBoard::board();
    ASSERT_TRUE(board->size() == boardSize);
    for (size_t i = 0; i < boardSize; ++i) {
        ASSERT_TRUE(board->at(i).size() == boardSize);
        for(size_t l = 0; l < boardSize; ++l) {
            ASSERT_TRUE(board->at(i)[l] == newBoard->at(i)[l]);
        }
    }
    board.reset();
    newBoard.reset();
    CPlayingBoard::deleteBoard();
}

TEST(Correct_board, place_unit) {
    CAttackingFactory attackingFactory = CAttackingFactory();
    CDefendingFactory defendingFactory = CDefendingFactory();
    std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
    ASSERT_TRUE(board->size() == boardSize);
    for (size_t i = 0; i < boardSize; ++i) {
        ASSERT_TRUE(board->at(i).size() == boardSize);
        for(size_t l = 0; l < boardSize; ++l) {
            ASSERT_TRUE(board->at(i)[l] == nullptr);
        }
    }
    CUnit* leaderA = attackingFactory.createLeader();
    CUnit* shooterD = defendingFactory.createShooter();
    CPlayingBoard::placeUnit(2, 3, leaderA);
    CPlayingBoard::placeUnit(3, 2, shooterD);
    for (size_t i = 0; i < boardSize; ++i) {
        ASSERT_TRUE(board->at(i).size() == boardSize);
        for(size_t l = 0; l < boardSize; ++l) {
            ASSERT_TRUE(board->at(i)[l] == nullptr ||
            (board->at(i)[l] != nullptr && i == 2 && l == 3 && board->at(i)[l]->getWarriorType() == leader &&
            board->at(i)[l]->getFraction() == attacking) ||
            (board->at(i)[l] != nullptr && i == 3 && l == 2 && board->at(i)[l]->getWarriorType() == shooter &&
            board->at(i)[l]->getFraction() == defending));
        }
    }
    board.reset();
    CPlayingBoard::deleteBoard();
}

TEST(Correct_unit, canMove) {
    CDefendingFactory defending = CDefendingFactory();
    CAttackingFactory attacking = CAttackingFactory();
    std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
    CUnit* ALeader = attacking.createLeader();
    CUnit* DLeader = defending.createLeader();
    CUnit* AInfantry = attacking.createInfantry();
    CUnit* DInfantry = defending.createInfantry();
    CUnit* AShooter = attacking.createShooter();
    CUnit* DShooter = defending.createShooter();
    CPlayingBoard::placeUnit(0, 1, ALeader);
    CPlayingBoard::placeUnit(0, 2, AInfantry);
    CPlayingBoard::placeUnit(0, 3, AShooter);
    CPlayingBoard::placeUnit(1, 2, DLeader);
    CPlayingBoard::placeUnit(1, 3, DInfantry);
    CPlayingBoard::placeUnit(4, 3, DShooter);

    ASSERT_TRUE(CPlayingBoard::canMove(0, 1, 0, 0));
    ASSERT_FALSE(CPlayingBoard::canMove(0, 1, 0, 2));
    ASSERT_TRUE(CPlayingBoard::canMove(0, 1, 1, 0));
    ASSERT_TRUE(CPlayingBoard::canMove(0, 1, 1, 1));
    ASSERT_FALSE(CPlayingBoard::canMove(0, 1, 4, 3));

    ASSERT_FALSE(CPlayingBoard::canMove(0, 2, 0, 3));
    ASSERT_FALSE(CPlayingBoard::canMove(0, 2, 0, 1));
    ASSERT_FALSE(CPlayingBoard::canMove(0, 2, 0, 2));
    ASSERT_FALSE(CPlayingBoard::canMove(0, 2, 1, 2));
    ASSERT_TRUE(CPlayingBoard::canMove(0, 2, 1, 1));

    ASSERT_FALSE(CPlayingBoard::canMove(0, 3, 0, 0));
    ASSERT_FALSE(CPlayingBoard::canMove(0, 3, 0, 1));
    ASSERT_FALSE(CPlayingBoard::canMove(0, 3, 0, 2));
    ASSERT_FALSE(CPlayingBoard::canMove(0, 3, 0, 3));
    ASSERT_TRUE(CPlayingBoard::canMove(0, 3, 0, 4));
    ASSERT_FALSE(CPlayingBoard::canMove(0, 3, 1, 4));
    ASSERT_FALSE(CPlayingBoard::canMove(0, 3, 1, 2));
    ASSERT_FALSE(CPlayingBoard::canMove(0, 3, 1, 3));
    ASSERT_FALSE(CPlayingBoard::canMove(0, 3, 4, 3));

    ASSERT_FALSE(CPlayingBoard::canMove(1, 2, 0, 3));
    ASSERT_FALSE(CPlayingBoard::canMove(1, 2, 0, 1));
    ASSERT_FALSE(CPlayingBoard::canMove(1, 2, 0, 2));
    ASSERT_FALSE(CPlayingBoard::canMove(1, 2, 1, 2));
    ASSERT_TRUE(CPlayingBoard::canMove(1, 2, 1, 1));
    ASSERT_TRUE(CPlayingBoard::canMove(1, 2, 2, 2));
    ASSERT_FALSE(CPlayingBoard::canMove(1, 2, 1, 3));
    ASSERT_FALSE(CPlayingBoard::canMove(1, 2, 4, 3));

    ASSERT_FALSE(CPlayingBoard::canMove(1, 3, 0, 3));
    ASSERT_FALSE(CPlayingBoard::canMove(1, 3, 0, 1));
    ASSERT_FALSE(CPlayingBoard::canMove(1, 3, 0, 2));
    ASSERT_FALSE(CPlayingBoard::canMove(1, 3, 1, 2));
    ASSERT_FALSE(CPlayingBoard::canMove(1, 3, 1, 3));
    ASSERT_TRUE(CPlayingBoard::canMove(1, 3, 1, 4));
    ASSERT_TRUE(CPlayingBoard::canMove(1, 3, 2, 2));
    ASSERT_FALSE(CPlayingBoard::canMove(1, 3, 4, 3));

    ASSERT_FALSE(CPlayingBoard::canMove(4, 3, 0, 2));
    ASSERT_FALSE(CPlayingBoard::canMove(4, 3, 0, 1));
    ASSERT_FALSE(CPlayingBoard::canMove(4, 3, 0, 3));
    ASSERT_FALSE(CPlayingBoard::canMove(4, 3, 1, 2));
    ASSERT_FALSE(CPlayingBoard::canMove(4, 3, 1, 3));
    ASSERT_FALSE(CPlayingBoard::canMove(4, 3, 4, 3));
    ASSERT_TRUE(CPlayingBoard::canMove(4, 3, 4, 2));

    CPlayingBoard::deleteBoard();
}

TEST(Correct_unit, canAttack) {
    CDefendingFactory defending = CDefendingFactory();
    CAttackingFactory attacking = CAttackingFactory();
    std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
    CUnit* ALeader = attacking.createLeader();
    CUnit* DLeader = defending.createLeader();
    CUnit* AInfantry = attacking.createInfantry();
    CUnit* DInfantry = defending.createInfantry();
    CUnit* AShooter = attacking.createShooter();
    CUnit* DShooter = defending.createShooter();
    CPlayingBoard::placeUnit(0, 1, ALeader);
    CPlayingBoard::placeUnit(0, 2, AInfantry);
    CPlayingBoard::placeUnit(0, 3, AShooter);
    CPlayingBoard::placeUnit(1, 2, DLeader);
    CPlayingBoard::placeUnit(1, 3, DInfantry);
    CPlayingBoard::placeUnit(4, 3, DShooter);

    ASSERT_FALSE(CPlayingBoard::canAttack(0, 1, 0, 0));
    ASSERT_FALSE(CPlayingBoard::canAttack(0, 1, 1, 0));
    ASSERT_FALSE(CPlayingBoard::canAttack(0, 1, 0, 1));
    ASSERT_FALSE(CPlayingBoard::canAttack(0, 1, 0, 2));
    ASSERT_TRUE(CPlayingBoard::canAttack(0, 1, 1, 2));
    ASSERT_TRUE(CPlayingBoard::canAttack(0, 1, 1, 3));
    ASSERT_FALSE(CPlayingBoard::canAttack(0, 1, 4, 3));

    ASSERT_FALSE(CPlayingBoard::canAttack(0, 2, 0, 3));
    ASSERT_FALSE(CPlayingBoard::canAttack(0, 2, 0, 1));
    ASSERT_FALSE(CPlayingBoard::canAttack(0, 2, 0, 2));
    ASSERT_TRUE(CPlayingBoard::canAttack(0, 2, 1, 2));
    ASSERT_FALSE(CPlayingBoard::canAttack(0, 2, 1, 3));
    ASSERT_FALSE(CPlayingBoard::canAttack(0, 2, 4, 3));

    ASSERT_FALSE(CPlayingBoard::canAttack(0, 3, 0, 0));
    ASSERT_FALSE(CPlayingBoard::canAttack(0, 3, 0, 1));
    ASSERT_FALSE(CPlayingBoard::canAttack(0, 3, 0, 2));
    ASSERT_FALSE(CPlayingBoard::canAttack(0, 3, 0, 3));
    ASSERT_TRUE(CPlayingBoard::canAttack(0, 3, 1, 2));
    ASSERT_TRUE(CPlayingBoard::canAttack(0, 3, 1, 3));
    ASSERT_TRUE(CPlayingBoard::canAttack(0, 3, 4, 3));

    ASSERT_FALSE(CPlayingBoard::canAttack(1, 2, 0, 3));
    ASSERT_FALSE(CPlayingBoard::canAttack(1, 2, 0, 1));
    ASSERT_FALSE(CPlayingBoard::canAttack(1, 2, 0, 2));
    ASSERT_FALSE(CPlayingBoard::canAttack(1, 2, 1, 2));
    ASSERT_FALSE(CPlayingBoard::canAttack(1, 2, 1, 1));
    ASSERT_FALSE(CPlayingBoard::canAttack(1, 2, 1, 3));
    ASSERT_FALSE(CPlayingBoard::canAttack(1, 2, 4, 3));

    ASSERT_TRUE(CPlayingBoard::canAttack(1, 3, 0, 3));
    ASSERT_FALSE(CPlayingBoard::canAttack(1, 3, 0, 1));
    ASSERT_FALSE(CPlayingBoard::canAttack(1, 3, 0, 2));
    ASSERT_FALSE(CPlayingBoard::canAttack(1, 3, 1, 2));
    ASSERT_FALSE(CPlayingBoard::canAttack(1, 3, 1, 3));
    ASSERT_FALSE(CPlayingBoard::canAttack(1, 3, 1, 4));
    ASSERT_FALSE(CPlayingBoard::canAttack(1, 3, 4, 3));

    ASSERT_FALSE(CPlayingBoard::canAttack(4, 3, 0, 2));
    ASSERT_FALSE(CPlayingBoard::canAttack(4, 3, 0, 1));
    ASSERT_TRUE(CPlayingBoard::canAttack(4, 3, 0, 3));
    ASSERT_FALSE(CPlayingBoard::canAttack(4, 3, 1, 2));
    ASSERT_FALSE(CPlayingBoard::canAttack(4, 3, 1, 3));
    ASSERT_FALSE(CPlayingBoard::canAttack(4, 3, 4, 3));
    ASSERT_FALSE(CPlayingBoard::canAttack(4, 3, 4, 2));

    CPlayingBoard::deleteBoard();
}

TEST(Correct_board, move_and_attack_unit) {
    std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
    CAttackingFactory attackingFactory = CAttackingFactory();
    CDefendingFactory defendingFactory = CDefendingFactory();
    CUnit* leaderA = attackingFactory.createLeader();
    CUnit* shooterD = defendingFactory.createShooter();
    CPlayingBoard::placeUnit(3, 3, leaderA);
    CPlayingBoard::placeUnit(3, 2, shooterD);
    ASSERT_TRUE(board->at(3)[3]);
    ASSERT_FALSE(board->at(2)[3]);
    CPlayingBoard::attack(3, 3, 3, 2);
    ASSERT_TRUE(board->at(3)[2]->getHealth() == -1);
    ASSERT_TRUE(board->at(3)[2]->isDead());
    CPlayingBoard::deleteBoard();
}

TEST(Correct_Node, add_child_remove_child) {
    CNode topNode = CNode(-1, 1, 1);
    ASSERT_TRUE(topNode.depth_ == 1);
    ASSERT_TRUE(topNode.addChild(-1, 2));
    ASSERT_FALSE(topNode.addChild(1, 2));
    CNode midNode = *topNode.children_[0].get();
    ASSERT_TRUE(midNode.depth_ == 2);
    ASSERT_FALSE(midNode.addChild(-1, 3));
    ASSERT_TRUE(midNode.addChild(1, 3));
    CNode downNode = *midNode.children_[0].get();
    ASSERT_TRUE(downNode.depth_ == 3);
    ASSERT_FALSE(downNode.addChild(1, 4));
    ASSERT_FALSE(downNode.addChild(1, 4));
    ASSERT_FALSE(midNode.removeChild(1, 4));
    ASSERT_TRUE(midNode.removeChild(1, 3));
    ASSERT_TRUE(midNode.children_.size() == 0);
    ASSERT_TRUE(topNode.children_[0]->addChild(1, 4));
    ASSERT_FALSE(topNode.removeChild(-1, 2));
}

TEST(Correct_Node, get_node) {
    CNode topNode = CNode(-1, 1, 1);
    topNode.addChild(-1, 2);
    topNode.addChild(-1, 3);
    topNode.children_[0]->addChild(1, 4);
    ASSERT_TRUE(topNode.getNode(1, 4) != nullptr);
    ASSERT_TRUE(topNode.getNode(1, 3) == nullptr);
}

TEST(Correct_board, composite_build_print_composite) {
    CAttackingFactory attackingFactory = CAttackingFactory();
    CDefendingFactory defendingFactory = CDefendingFactory();
    std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
    CUnit* leaderA = attackingFactory.createLeader();
    CUnit* shooterD = defendingFactory.createShooter();
    CPlayingBoard::placeUnit(2, 3, leaderA);
    CPlayingBoard::placeUnit(3, 2, shooterD);
    CComposite attackingComposite(attacking);
    CComposite defendingComposite(defending);
    attackingComposite.printComposite(); //checked for correct printing
    CPlayingBoard::deleteBoard();
}

TEST(Correct_board, composite_get_node_get_parent_node) {
    CAttackingFactory attackingFactory = CAttackingFactory();
    CDefendingFactory defendingFactory = CDefendingFactory();
    std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
    CUnit* leaderA = attackingFactory.createLeader();
    CUnit* shooterD = defendingFactory.createShooter();
    CUnit* infantryD = defendingFactory.createInfantry();
    CPlayingBoard::placeUnit(2, 3, leaderA);
    CPlayingBoard::placeUnit(3, 2, shooterD);
    CPlayingBoard::placeUnit(3, 3, infantryD);
    CComposite defendingComposite(defending);
    std::shared_ptr<CNode> top = defendingComposite.topNode_;
    std::shared_ptr<CNode> mid = top->children_[0];
    std::shared_ptr<CNode> bottom1 = mid->children_[0];
    std::shared_ptr<CNode> bottom2 = mid->children_[1];
    ASSERT_TRUE(defendingComposite.getNode(bottom1->savedComponent_.first, bottom1->savedComponent_.second) == bottom1);
    ASSERT_TRUE(defendingComposite.getNode(bottom2->savedComponent_.first, bottom2->savedComponent_.second) == bottom2);
    ASSERT_TRUE(defendingComposite.getParentNode(bottom1->savedComponent_.first, bottom1->savedComponent_.second) == mid);
    ASSERT_TRUE(defendingComposite.getNode(mid->savedComponent_.first, mid->savedComponent_.second) == mid);
    ASSERT_TRUE(defendingComposite.getParentNode(mid->savedComponent_.first, mid->savedComponent_.second) == top);
    ASSERT_TRUE(defendingComposite.getNode(top->savedComponent_.first, top->savedComponent_.second) == top);
    ASSERT_TRUE(defendingComposite.getParentNode(top->savedComponent_.first, top->savedComponent_.second) == nullptr);
    CPlayingBoard::deleteBoard();
}

TEST(Correct_board, composite_adding_deleting_editing) {
    CAttackingFactory attackingFactory = CAttackingFactory();
    CDefendingFactory defendingFactory = CDefendingFactory();
    std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
    CUnit* leaderA = attackingFactory.createLeader();
    CUnit* shooterD = defendingFactory.createShooter();
    CUnit* infantryD = defendingFactory.createInfantry();
    CPlayingBoard::placeUnit(2, 3, leaderA);
    CPlayingBoard::placeUnit(3, 2, shooterD);
    CPlayingBoard::placeUnit(3, 3, infantryD);
    CComposite defendingComposite(defending);
    ASSERT_TRUE(defendingComposite.addChild(1));
    ASSERT_FALSE(defendingComposite.addChild(2));
    ASSERT_FALSE(defendingComposite.removeChild(-1, 1));
    ASSERT_FALSE(defendingComposite.removeChild(-1, 2));
    ASSERT_TRUE(defendingComposite.switchChild(3, 3, 3));
    ASSERT_TRUE(defendingComposite.switchChild(3, 2, 3));
    ASSERT_TRUE(defendingComposite.removeChild(-1, 2));
    ASSERT_TRUE(defendingComposite.addChild(1));
    std::shared_ptr<CNode> ptr = defendingComposite.topNode_;
    ASSERT_TRUE(ptr->children_.size() == 2);
    std::shared_ptr<CNode> mid3 = defendingComposite.topNode_->children_[0];
    std::shared_ptr<CNode> mid2 = defendingComposite.topNode_->children_[1];
    ASSERT_TRUE(mid3->children_.size() == 2);
    ASSERT_TRUE(mid3->savedComponent_.first == -1 && mid3->savedComponent_.second == 3);
    ASSERT_TRUE(mid2->children_.empty());
    ASSERT_TRUE(mid2->savedComponent_.first == -1 && mid2->savedComponent_.second == 2);
    std::shared_ptr<CNode> bottom1 = mid3->children_[0];
    std::shared_ptr<CNode> bottom2 = mid3->children_[1];
    ASSERT_TRUE(bottom1->children_.empty());
    ASSERT_TRUE(bottom2->children_.empty());
    ASSERT_TRUE(bottom1->savedComponent_.first == 3 && bottom1->savedComponent_.second == 3);
    ASSERT_TRUE(bottom2->savedComponent_.first == 3 && bottom2->savedComponent_.second == 2);
    CPlayingBoard::deleteBoard();
}

TEST(Correct_board, composite_moving) {
    CAttackingFactory attackingFactory = CAttackingFactory();
    CDefendingFactory defendingFactory = CDefendingFactory();
    std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
    CUnit* leaderA = attackingFactory.createLeader();
    CUnit* shooterD = defendingFactory.createShooter();
    CUnit* infantryD = defendingFactory.createInfantry();
    CPlayingBoard::placeUnit(2, 3, leaderA);
    CPlayingBoard::placeUnit(3, 2, shooterD);
    CPlayingBoard::placeUnit(3, 3, infantryD);
    CComposite defendingComposite(defending);
    ASSERT_TRUE(CPlayingBoard::canMoveComposite(defendingComposite, std::make_pair(-1, 2), 1, 0));
    ASSERT_TRUE(CPlayingBoard::canMoveComposite(defendingComposite, std::make_pair(-1, 2), 0, 1));
    ASSERT_TRUE(CPlayingBoard::canMoveComposite(defendingComposite, std::make_pair(-1, 1), 1, 0));
    ASSERT_FALSE(CPlayingBoard::canMoveComposite(defendingComposite, std::make_pair(-1, 2), 1, 1));
    ASSERT_FALSE(CPlayingBoard::canMoveComposite(defendingComposite, std::make_pair(3, 2), 1, 1));
    ASSERT_TRUE(CPlayingBoard::canMoveComposite(defendingComposite, std::make_pair(3, 3), 1, 1));
    std::shared_ptr<CNode> top = defendingComposite.topNode_;
    std::shared_ptr<CNode> mid = top->children_[0];
    std::shared_ptr<CNode> bottom1 = mid->children_[0];
    std::shared_ptr<CNode> bottom2 = mid->children_[1];
    CPlayingBoard::moveComposite(-1, 2, 1, 0, defendingComposite);
    ASSERT_TRUE(bottom1->getSavedComponent() == std::make_pair(4, 2));
    ASSERT_TRUE(bottom2->getSavedComponent() == std::make_pair(4, 3));
    CPlayingBoard::moveComposite(-1, 2, 0, 1, defendingComposite);
    CPlayingBoard::moveComposite(-1, 2, 1, 1, defendingComposite);
    ASSERT_TRUE(bottom1->getSavedComponent() == std::make_pair(4, 3));
    ASSERT_TRUE(bottom2->getSavedComponent() == std::make_pair(4, 4));
    defendingComposite.printComposite();
    CPlayingBoard::deleteBoard();
}

TEST(CVisitor, printBoard) {
    CAttackingFactory attackingFactory = CAttackingFactory();
    CDefendingFactory defendingFactory = CDefendingFactory();
    std::shared_ptr<std::vector<std::vector<CUnit*> > > board = CPlayingBoard::board();
    CUnit* leaderA = attackingFactory.createLeader();
    CUnit* shooterD = defendingFactory.createShooter();
    CUnit* infantryD = defendingFactory.createInfantry();
    CPlayingBoard::placeUnit(2, 3, leaderA);
    CPlayingBoard::placeUnit(3, 2, shooterD);
    CPlayingBoard::placeUnit(3, 3, infantryD);
    CPlayingBoard::printBoard();
    CPlayingBoard::deleteBoard();
    ASSERT_TRUE(true); // вывод корректный
}