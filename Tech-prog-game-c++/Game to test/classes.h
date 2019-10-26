#include <string>
#include <vector>
#include <memory>
#include <cstddef>
#include <set>
#include "gtest/gtest_prod.h"

const int boardSize = 8;
const int maxCompositeDepth = 3;
const int attackingUnits = 3; // без учёта короля
const int defendingUnits = 3; // без учёта короля

std::vector<std::string> structureNames{"Army", "Squad", "Soldier"}; // names of the levels, size should be equal to maxCompositeDepth

class CUnit;
class CPlayingBoard;
class CVisitor;
class CNode;

enum fraction {defending, attacking};
enum warriorType {leader, infantry, shooter};

bool betweenBorders(int, int, int, int, int, int);

bool insideBattleField(int, int, std::shared_ptr<std::vector<std::vector<CUnit*> > >);

class CArmyFactory {
public:
    CArmyFactory() = default;
    virtual ~CArmyFactory() = default;
    virtual CUnit* createLeader() const = 0;
    virtual CUnit* createInfantry() const = 0;
    virtual CUnit* createShooter() const = 0;
};

class CAttackingFactory: public CArmyFactory {
public:
    CAttackingFactory() = default;
    ~CAttackingFactory() override = default;
    CUnit* createLeader() const override;
    CUnit* createInfantry() const override;
    CUnit* createShooter() const override;
};

class CDefendingFactory: public CArmyFactory {
public:
    CDefendingFactory() = default;
    ~CDefendingFactory() override = default;
    CUnit* createLeader() const override;
    CUnit* createInfantry() const override;
    CUnit* createShooter() const override;
};

class CUnit {
private:
    int health_;
    int damage_;
    fraction fraction_;
    warriorType type_;

    void reduceHealth(int);

    friend class CPlayingBoard;
    friend class CComposite;
    FRIEND_TEST(Correct_factory, defending_units);
    FRIEND_TEST(Correct_factory, attacking_units);
    FRIEND_TEST(Correct_board, place_unit);
    FRIEND_TEST(Correct_unit, canAttack_and_canMove);
    FRIEND_TEST(Correct_board, move_and_attack_unit);
    FRIEND_TEST(Correct_factory, decorator);
    FRIEND_TEST(Correct_board, composite_build_print_composite);
public:
    CUnit(int, int, fraction, warriorType);
    virtual ~CUnit() = default;
    CUnit(const CUnit&);
    CUnit& operator= (const CUnit&);

    virtual void visit(CVisitor) const = 0;
    virtual bool canMove(int, int, int, int) const = 0;
    virtual bool canAttack(int, int, int, int) const = 0;
    int getHealth() const;
    const int getDamage() const;
    warriorType getWarriorType() const;
    fraction getFraction() const;
    bool isDead() const;
};

class CAttackingLeader: public CUnit {
private:
    CAttackingLeader();
    friend CUnit* CAttackingFactory::createLeader() const;
public:
    ~CAttackingLeader() override = default;

    void visit(CVisitor) const override;
    bool canMove(int, int, int, int) const override;
    bool canAttack(int, int, int, int) const override;
};

class CDefendingLeader: public CUnit {
private:
    CDefendingLeader();
    friend CUnit* CDefendingFactory::createLeader() const;
public:
    ~CDefendingLeader() override = default;

    void visit(CVisitor) const override;
    bool canMove(int, int, int, int) const override;
    bool canAttack(int, int, int, int) const override;
};

class CAttackingInfantry: public CUnit {
private:
    CAttackingInfantry();
    friend CUnit* CAttackingFactory::createInfantry() const;
public:
    ~CAttackingInfantry() override = default;

    void visit(CVisitor) const override;
    bool canMove(int, int, int, int) const override;
    bool canAttack(int, int, int, int) const override;
};

class CDefendingInfantry: public CUnit {
private:
    CDefendingInfantry();
    friend CUnit* CDefendingFactory::createInfantry() const;

public:
    ~CDefendingInfantry() override = default;

    void visit(CVisitor) const override;
    bool canMove(int, int, int, int) const override;
    bool canAttack(int, int, int, int) const override;
};

class CAttackingShooter: public CUnit {
private:
    CAttackingShooter();
    friend CUnit* CAttackingFactory::createShooter() const;
public:
    ~CAttackingShooter() override = default;

    void visit(CVisitor) const override;
    bool canMove(int, int, int, int) const override;
    bool canAttack(int, int, int, int) const override;
};

class CDefendingShooter: public CUnit {
private:
    CDefendingShooter();
    friend CUnit* CDefendingFactory::createShooter() const;
public:
    ~CDefendingShooter() override = default;

    void visit(CVisitor) const override;
    bool canMove(int, int, int, int) const override;
    bool canAttack(int, int, int, int) const override;
};

class CComposite;

class CPlayingBoard {
private:
    static bool canPlaceUnit(int, int);
    static bool canMove(int, int, int, int);
    static bool canMoveComposite(const CComposite&, std::pair<int, int>, int, int);
    static bool canAttack(int, int, int, int);
    static bool canAttack(int, int);

    static std::shared_ptr<std::vector<std::vector<CUnit*> > > desk_;

    friend class CGame;

    FRIEND_TEST(Correct_unit, canAttack);
    FRIEND_TEST(Correct_unit, canMove);
    FRIEND_TEST(Correct_board, composite_moving);
public:
    CPlayingBoard() = delete;
    ~CPlayingBoard();
    CPlayingBoard(const CPlayingBoard&) = delete;
    CPlayingBoard& operator=(const CPlayingBoard&) = delete;

    static std::shared_ptr<std::vector<std::vector<CUnit*> > > board();
    static void moveComposite(int, int, int, int, CComposite);
    static bool allMovedComposite(std::shared_ptr<CNode>, int);
    static bool allUnmovedComposite(std::shared_ptr<CNode>);
    static void placeUnit(int, int, CUnit*);
    static void attack(int, int, int, int);
    static void deleteBoard();
    static void printBoard();
};

class CFactoryDecorator: public CArmyFactory {
public:
    CFactoryDecorator(CArmyFactory*);
    ~CFactoryDecorator() override = default;

    CUnit* createLeader() const override;
    CUnit* createInfantry() const override;
    CUnit* createShooter() const override;

    std::shared_ptr<CArmyFactory> controlledFactory;
};


class CNode {
private:
    int depth_; // top depth = 1
    std::vector<std::shared_ptr<CNode> > children_;
    std::pair<int, int> savedComponent_;
    bool moveOnTheIteration;
public:
    CNode(int, int, int);
    ~CNode() = default;

    bool addChild(int, int);
    bool removeChild(int, int);
    std::pair<int, int> getSavedComponent() const;
    std::shared_ptr<CNode> getNode(int, int) const;
    std::shared_ptr<CNode> getParentNode(int, int, std::shared_ptr<CNode>) const;

    friend class CComposite;
    friend class CPlayingBoard;

    FRIEND_TEST(Correct_board, composite_moving);
    FRIEND_TEST(Correct_Node, add_child_remove_child);
    FRIEND_TEST(Correct_Node, get_node);
    FRIEND_TEST(Correct_board, composite_get_node_get_parent_node);
    FRIEND_TEST(Correct_board, composite_adding_deleting_editing);
};

bool operator <(const std::shared_ptr<CNode>&, const std::shared_ptr<CNode>&);

struct CComparator {
public:
    bool operator ()(const std::shared_ptr<CNode>&, const std::shared_ptr<CNode>&);
};

class CComposite {
private:
    std::shared_ptr<CNode> topNode_;
    fraction fraction_;
    std::set<int> usedNumbers_;
public:
    CComposite(fraction);
    ~CComposite() = default;

    void printComposite() const;
    void startNewMove();
    std::shared_ptr<CNode> getTopNode() const;
    std::shared_ptr<CNode> getNode(int, int) const;
    std::shared_ptr<CNode> getParentNode(int, int) const;
    bool addChild(int);
    bool removeChild(int, int);
    bool switchChild(int, int, int);
    size_t size() const;

    FRIEND_TEST(Correct_board, composite_get_node_get_parent_node);
    FRIEND_TEST(Correct_board, composite_moving);
    FRIEND_TEST(Correct_board, composite_adding_deleting_editing);
};

class CVisitor {
public:
    CVisitor() = default;
    ~CVisitor() = default;

    void visit(CDefendingInfantry) const;
    void visit(CDefendingShooter) const;
    void visit(CDefendingLeader) const;
    void visit(CAttackingInfantry) const;
    void visit(CAttackingShooter) const;
    void visit(CAttackingLeader) const;
};

class CGame {
private:
    bool gameFinished;
    fraction winner;

    CArmyFactory* attackingFactory;
    CArmyFactory* defendingFactory;
    CComposite attackingComposite;
    CComposite defendingComposite;

    std::pair<int, int> tryPlaceUnit() const;
    void placeLeader(fraction);
    void placeUnit(fraction, int);
    void makeMove(fraction);
    void makeMovePhase(fraction);
    void makeAttack(fraction);
    void makeAttackPhase(fraction);
    void placeArmy();
    void makeEditCompositePhase(fraction);
    void makeEditComposite(fraction);
public:
    CGame();
    ~CGame();
    void game();
};