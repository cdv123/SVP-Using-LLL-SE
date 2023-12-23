import numpy as np

class EnumartionTree:

    def __init__(self, basis):

        self.curr_min = min(get_distance(vector) for vector in basis)
        self.basis = basis
        self.dim = len(basis)

    def update_min(self, point):
        distance = get_distance(point)
        if distance == 0:
            return
        if distance < self.curr_min:
            self.curr_min = distance

    def in_bound(self, point):

        if len(point) == 0:
            return True
        
        if get_distance(point) < self.curr_min:
            return True
        
        return False
    
    def dfs(self, curr_scalars):

        print(curr_scalars)
        point = [np.dot(curr_scalars[i], self.basis[i]) for i in range(len(curr_scalars))]
        if len(point) > 0:
            point = np.sum(point, axis=0)

        if len(curr_scalars) == self.dim:
            self.update_min(point)
        
        if not self.in_bound(point):
            return 

        if len(curr_scalars) < self.dim:

            new_scalars = curr_scalars[:]
            new_scalars.append(0)
            self.dfs(new_scalars)

        if len(curr_scalars) == 0:
            return
        
        while self.in_bound(point):

            curr_scalars[-1]*=-1
            if curr_scalars[-1] >= 0:
                curr_scalars[-1]+=1

            point = [np.dot(curr_scalars[i], self.basis[i]) for i in range(len(curr_scalars))]
            if len(point) > 0:
                point = np.sum(point, axis=0)

            self.dfs(curr_scalars)

def euclidean_norm(v):
    return np.sqrt(np.sum(v**2))

def euclidean_norm2(v):
    return np.sum(v**2)

def LLL(basis,delta):
    new_basis = gram_schmidt(basis)
    pass

def gram_schmidt(basis):
    new_basis = np.zeros((len(basis),len(basis[0])))
    new_basis[0] = basis[0][:]
    for i in range(1,len(basis)):
        new_basis[i] = basis[i] - proj(new_basis,basis[i],i-1)
    return new_basis


def proj(new_basis,u,k):
    new_v = np.zeros(len(new_basis[0]))
    for i in range(k+1):
        new_v+= np.multiply(np.dot(new_basis[i],u)/(euclidean_norm2(new_basis[i])),new_basis[i])
    return new_v

def get_distance(vector):
    return np.sum(np.square(vector))

def solve_svp(basis):

    enum_tree = EnumartionTree(basis)

    enum_tree.dfs([])

    return np.sqrt(enum_tree.curr_min)

# curr_scalars = [1]
# basis = [[14800322531141549294, 1, 0, 0, 0, 0, 0, 0, 0, 0], [17796564942578567201, 0, 1, 0, 0, 0, 0, 0, 0, 0], [7301743052718098431, 0, 0, 1, 0, 0, 0, 0, 0, 0], [5792622719448160275, 0, 0, 0, 1, 0, 0, 0, 0, 0], [13825670105792957044, 0, 0, 0, 0, 1, 0, 0, 0, 0], [7338474235679864976, 0, 0, 0, 0, 0, 1, 0, 0, 0], [6466088670324559620, 0, 0, 0, 0, 0, 0, 1, 0, 0], [5315957138953616152, 0, 0, 0, 0, 0, 0, 0, 1, 0], [7038974009087367626, 0, 0, 0, 0, 0, 0, 0, 0, 1], [13662771198155157402, 0, 0, 0, 0, 0, 0, 0, 0, 0]]
# basis = [[3462528728, 444363041, 953498107, 3019162, 293072558, 454157756, 1950819278, 2174448309, 2956307025, 2050440410], [1676991435, 2190045933, 3360605833, 3782131535, 1076691369, 1286267085, 3124954673, 1356441824, 1675223571, 3617515077], [3467837865, 2727897666, 2512785299, 3013662465, 108820214, 2883385007, 1068246967, 798890469, 2874893319, 279879480], [3036791821, 956443849, 199347373, 1643597530, 1622084324, 471638746, 1055553070, 2020155791, 1772672903, 179014643], [3726309520, 3416039174, 816778328, 4178510478, 1052560161, 2365858231, 3810589638, 1478643591, 3537405930, 1196123656], [2101281032, 731853286, 35290828, 1463099681, 62123405, 1905949073, 3166373256, 2899371508, 674265594, 3891491225], [2924313520, 4018395377, 1444789986, 2465339761, 1288445553, 3964167277, 3710424026, 2819673876, 103363586, 3597674584], [3237227071, 578279073, 119471827, 325475776, 1510452858, 210663454, 2633248087, 2140229824, 153673582, 2811110910], [1566590271, 1264942241, 750770120, 1308334483, 1808072791, 1483343336, 4170849932, 3261787946, 1391377726, 2865208703], [1950175842, 47002310, 377036287, 1445470496, 1706137745, 2166273829, 1896680823, 2831856618, 2825133614, 3610138421]]
basis = [[2,0,1], [0,2,0], [1,0,2]]
print(solve_svp(basis))
# print([np.dot(curr_scalars[i], basis[i]) for i in range(1)])
    