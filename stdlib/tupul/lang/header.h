#pragma once
#include "../../../data/TupulField.h"
#include "../../../data/Types.h"
#include "../../../TupulMethod.h"
#include "../../../TupulClass.h"
#include "../../../Definitions.h"
#include "../../../Locals.h"
#include "../../../Utils.h"

TupulMethod* makeSTDMethod(TupulClass* clazz, string name, string descr, TupulByte** (*exec)(TupulMethod*,Locals*));
