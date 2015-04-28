#ifndef BULLETATTACHLIST_H
#define BULLETATTACHLIST_H

#include "entityattachlist.h"
#include "bulletbase.h"

class BulletAttachList : public EntityAttachList<BulletBase>
{
public:

    BulletAttachList(BulletList* list);

    // more functions to do ...

};

#endif // BULLETATTACHLIST_H
