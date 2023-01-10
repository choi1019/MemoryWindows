#include "Directory.h"

Directory Directory::s_dirObjects(Directory::EType::eObject);
Directory Directory::s_dirComponents(Directory::EType::eComponent);
Directory Directory::s_dirEvents(Directory::EType::eEvent);
Directory Directory::s_dirExceptions(Directory::EType::eException);