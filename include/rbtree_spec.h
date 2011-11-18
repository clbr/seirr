/*
 * Specializations of the ll-r-b tree
 *
 * (C) Lauri Kasanen
 * Under the zlib license.
*/

#include "irrMap.h"
#include "irrArray.h"
#include "IAnimatedMesh.h"

#ifndef irrMap_SPEC_H
#define irrMap_SPEC_H

namespace irr
{
namespace core
{

template <typename K, typename D> class deltree : public map <K, D> {

protected:

	void nuke(typename map <K, D>::Node *n) {
		delete n->getValue();
		map<K, D>::nuke(n);
	}

};

template <typename K, typename D> class droptree : public map <K, D> {

public:
	core::array<io::SNamedPath> *findUnused() {
		core::array<io::SNamedPath> *a = new core::array<io::SNamedPath>;
		if (map <K, D>::Root) findunused(map <K, D>::Root, a);

		return a;
	}

protected:

	void nuke(typename map <K, D>::Node *n) {
		n->getValue()->drop();
		map<K, D>::nuke(n);
	}

	void findunused(typename map <K, D>::Node *n, core::array<io::SNamedPath> *a) {
		if (n->getValue()->getReferenceCount() == 1) a->push_back(n->getKey());

		if (n->LeftChild) findunused(n->LeftChild, a);
		if (n->RightChild) findunused(n->RightChild, a);
	}
};

} // end namespace core

typedef core::map<io::SNamedPath, scene::IAnimatedMesh *>::Node meshnode;
typedef core::map<const scene::IAnimatedMesh *, const io::SNamedPath *>::Node revnode;
typedef core::map<const scene::IMesh *, const io::SNamedPath *>::Node inode;

typedef core::map<io::SNamedPath, video::ITexture *>::Node texnode;
typedef core::map<const video::ITexture *, const io::SNamedPath *>::Node revtexnode;

} // end namespace irr

#endif
