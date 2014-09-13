class VAC_ActorManager;
class Partial_VAC_World:public BZK_World
{
public:
virtual int GetSectorByPos(BZK_Vec3f &aVec,int aHint=1,int aTriggerHint=TRIG_NOP)
{
std::cout << "incomplete class-method called" <<std::endl;
return 0;
}
};
//extern void GameTick(VAC_ActorManager &aPeople,VAC_World &aWorld);
//extern void GameInit(VAC_ActorManager &aPeople,VAC_World &aWorld);
//extern void GameStart(VAC_ActorManager &aPeople,VAC_World &aWorld);
//extern void GameRestart(VAC_ActorManager &aPeople,VAC_World &aWorld);
