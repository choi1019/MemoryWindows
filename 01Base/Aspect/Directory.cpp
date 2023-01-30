#include "Directory.h"

Directory Directory::s_dirObjects(Directory::EType::eObject);
Directory Directory::s_dirComponents(Directory::EType::eComponent);
Directory Directory::s_dirEvents(Directory::EType::eEvent);
Directory Directory::s_dirExceptions(Directory::EType::eException);

void Directory::ClearDirectories() {
	Directory::s_dirObjects.Clear();
	Directory::s_dirComponents.Clear();
	Directory::s_dirEvents.Clear();
	Directory::s_dirExceptions.Clear();
};