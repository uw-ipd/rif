from rif import stripe_index_3d, rcl
from rosetta.numeric import xyzVector_float_t
from rosetta.core.kinematics import Stub
from rosetta.core.id import AtomID


def get_rosetta_stubs(pose, cen, aname1, aname2, aname3):
    centers, stubs = [], []
    for ir in range(1, pose.size() + 1):
        r = pose.residue(ir)
        if not (r.has(cen) and r.has(aname1) and r.has(aname2) and
                r.has(aname3)):
            continue
        centers.append(r.xyz(cen))
        stub = Stub(r.xyz(aname1), r.xyz(aname2), r.xyz(aname3))
        stubs.append((ir, stub))
    return centers, stubs


class PoseStubIndex(object):
    def __init__(self, pose, clash_radius=3.2, contact_radius=7.0, nbr_atom='CA'):
        bb_atoms = rcl.atoms(pose, 'bbheavy')
        cens, stubs = get_rosetta_stubs(pose, 'CA', 'N', 'CA', 'C')
        self.clash_check = stripe_index_3d(clash_radius, bb_atoms)
        self.stub_map = stripe_index_3d(contact_radius, cens, stubs)
        self.nbr_atom = nbr_atom

    def clashes(self, pose):
        for ir in range(1, pose.size() + 1):
            for ia in range(1, pose.residue(ir).nheavyatoms() + 1):
                xyz = pose.xyz(AtomID(ia, ir))
                if self.clash_check.neighbor_exists(xyz):
                    return True
        return False

    def contacting_stubs(self, pose):
        if self.clashes(pose):
            return None  # could alternately return empty set
        contacts = set()
        for ir in range(1, pose.size() + 1):
            res = pose.residue(ir)
            if res.has(self.nbr_atom):
                xyz = res.xyz(self.nbr_atom)
                nbrs = self.stub_map.neighbors(xyz)
                contacts.update(nbrs)
        return contacts
