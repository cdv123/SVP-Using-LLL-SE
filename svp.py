import numpy as np
import math

class EnumartionTree:

    def __init__(self, basis):

        self.curr_min = min(np.abs([get_distance(vector) for vector in basis]))
        # self.search_area = get_search_area(basis)
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
        
        if get_distance(point) < self.search_area:
            return True
        
        return False
    
    def dfs(self, curr_scalars):
        
        point = [np.dot(curr_scalars[i], self.basis[i]) for i in range(len(curr_scalars))]

        if len(point) > 0:
            point = np.sum(point, axis=0)

        if len(curr_scalars) == self.dim:
            self.update_min(point)
            return
        
        if not self.in_bound(point):
            return 

        val = 0

        while self.in_bound(point):

            curr_scalars.append(val)
            self.dfs(curr_scalars)
            point = [np.dot(curr_scalars[i], self.basis[i]) for i in range(len(curr_scalars))]
            curr_scalars.pop()
            
            val*=-1
            if val >= 0:
                val+=1

        if len(curr_scalars) == 0:
            return
        
    def gram_schmidt(self):

        mu = np.zeros((self.dim, self.dim))
        # ortho_basis = self.basis[::]
        ortho_basis = np.zeros((self.dim, self.dim))
        # ortho_basis[0] = self.basis[0][:]



        for i in range(self.dim):
            for j in range(self.dim):
                mu[i][j] = 0
                ortho_basis[i][j] = self.basis[i][j]
            for k in range(i):
                # print(np.dot(ortho_basis[k], ortho_basis[k]))
                mu[i][k] = np.dot(self.basis[i], ortho_basis[k])/np.dot(ortho_basis[k], ortho_basis[k])
                for j in range(self.dim):
                    ortho_basis[i][j]-=mu[i][k]*ortho_basis[k][j]
        # print(self.basis)
        # print(mu)
        # print(ortho_basis)
        for i in range(self.dim):
            mu[i][i] = 1
        
        # for i in range(self.dim):
        #     for j in range(self.dim):
        #         ortho_basis[i][j] = round(ortho_basis[i][j])

        # print(np.dot(ortho_basis, mu))
        # print(ortho_basis)
        # print(mu_ij)x 

        return ortho_basis, mu

    # def gram_schmidt(self):
    #     new_basis = np.zeros((self.dim, self.dim))
    #     mu = np.zeros((self.dim, self.dim))
    #     new_basis[0] = basis[0][:]
    #     for i in range(1,len(basis)):
    #         new_basis[i] = basis[i] - proj(new_basis,basis[i],i-1,mu)
    #     return new_basis, mu


    def SE(self):

        # compute Gram Schmidt information of basis
        gs_basis, mu_ij = self.gram_schmidt()
        self.search_area = get_search_area(gs_basis)
        # self.search_area = np.dot(basis[0], basis[0])
        # self.search_area = min([get_distance(self.basis[i]) for i in range(len(self.basis))])
        # print(type(self.search_area))
        # print(self.search_area)
        # self.search_area = min(self.search_area)**2
        # return
        # print(mu_ij, "\n", gs_basis)
        # initialise variables
        p = np.zeros(self.dim+1)
        v = np.zeros(self.dim)
        c = np.zeros(self.dim)
        w = np.zeros(self.dim)
        v[0] = 1 
        k = 0
        last_nonzero = 0

        while True:
            p[k] = p[k+1] + ((v[k]- c[k])**2) * np.matmul(gs_basis[k], gs_basis[k])
            if p[k] < self.search_area:
                # print(p[k], self.search_area**2)
                if k == 0:
                    self.search_area = p[k]
                else:
                    k-=1
                    c[k] = 0
                    for i in range(k+1, self.dim):
                        c[k]-= mu_ij[i][k]*v[i]
                    w[k] = 1
                    v[k] = round(c[k])
            
            else:
                k+=1
                # print(k, self.search_area)
                if k == self.dim:   
                    return math.sqrt(self.search_area)
                if k >= last_nonzero:      
                    last_nonzero = k
                    v[k]+=1
                else:
                    if v[k] > c[k]:
                        v[k]-=w[k]
                    else:
                        v[k]+=w[k]
                    w[k]+=1

    def LLL(self):
        gs_basis, mu_ij = self.gram_schmidt()
        delta = 0.75
        k = 1

        while k < self.dim:
            self.size_reduction(mu_ij, k)
            if np.dot(gs_basis[k], gs_basis[k]) < (delta - mu_ij[k][k-1]**2)*np.dot(gs_basis[k-1], gs_basis[k-1]):
                temp = self.basis[k][:]
                temp2 = self.basis[k-1][:]
                self.basis[k] = temp2
                self.basis[k-1] = temp
                gs_basis, mu_ij = self.gram_schmidt()
                k = max(k-1, 1)
            else:
                k+=1
    
    def size_reduction(self, mu, k):
        for j in range(k-1, -1, -1):
            print(mu[k][j])
            if abs(mu[k][j]) > 0.5:
                self.basis[k] = np.subtract(self.basis[k], round(mu[k][j])*np.array(self.basis[j]))
                print(mu[k][j])
                mu[k][j] = mu[k][j] - round(mu[k][j])
                for i in range(j):
                    mu[k][i] = mu[k][i] - round(mu[k][j])*mu[j][i]
        print(mu)
        print(self.basis)
def euclidean_norm(v):
    return np.sqrt(np.sum(v**2))

def euclidean_norm2(v):
    return np.sum(v**2)

def proj(new_basis,u,k,mu):
    new_v = np.zeros(len(new_basis[0]))
    for i in range(k+1):
        mu[k][i] = np.dot(new_basis[i],u)/(euclidean_norm2(new_basis[i]))
        new_v+= np.multiply(np.dot(new_basis[i],u)/(euclidean_norm2(new_basis[i])),new_basis[i])
    return new_v

def get_distance(vector):
    return np.sum(np.square(vector))


def get_search_area(basis):

    gamma_n = 1+len(basis)/4
    coefficient = math.sqrt(gamma_n)
    vol_L = 1
    for i in basis:
        vol_L = vol_L * math.sqrt(np.dot(i, i))

    inverse_vol = vol_L**(1/len(basis))

    return (coefficient * inverse_vol)**2
 
def solve_svp(basis):

    enum_tree = EnumartionTree(basis)

    print(enum_tree.LLL())

    # print(enum_tree.basis)

    # return np.sqrt(enum_tree.curr_min)

# curr_scalars = [1]
# basis = [[14800322531141549294, 1, 0, 0, 0, 0, 0, 0, 0, 0], [17796564942578567201, 0, 1, 0, 0, 0, 0, 0, 0, 0], [7301743052718098431, 0, 0, 1, 0, 0, 0, 0, 0, 0], [5792622719448160275, 0, 0, 0, 1, 0, 0, 0, 0, 0], [13825670105792957044, 0, 0, 0, 0, 1, 0, 0, 0, 0], [7338474235679864976, 0, 0, 0, 0, 0, 1, 0, 0, 0], [6466088670324559620, 0, 0, 0, 0, 0, 0, 1, 0, 0], [5315957138953616152, 0, 0, 0, 0, 0, 0, 0, 1, 0], [7038974009087367626, 0, 0, 0, 0, 0, 0, 0, 0, 1], [13662771198155157402, 0, 0, 0, 0, 0, 0, 0, 0, 0]]
# basis = np.array([[3462528728, 444363041, 953498107, 3019162, 293072558, 454157756, 1950819278, 2174448309, 2956307025, 2050440410], [1676991435, 2190045933, 3360605833, 3782131535, 1076691369, 1286267085, 3124954673, 1356441824, 1675223571, 3617515077], [3467837865, 2727897666, 2512785299, 3013662465, 108820214, 2883385007, 1068246967, 798890469, 2874893319, 279879480], [3036791821, 956443849, 199347373, 1643597530, 1622084324, 471638746, 1055553070, 2020155791, 1772672903, 179014643], [3726309520, 3416039174, 816778328, 4178510478, 1052560161, 2365858231, 3810589638, 1478643591, 3537405930, 1196123656], [2101281032, 731853286, 35290828, 1463099681, 62123405, 1905949073, 3166373256, 2899371508, 674265594, 3891491225], [2924313520, 4018395377, 1444789986, 2465339761, 1288445553, 3964167277, 3710424026, 2819673876, 103363586, 3597674584], [3237227071, 578279073, 119471827, 325475776, 1510452858, 210663454, 2633248087, 2140229824, 153673582, 2811110910], [1566590271, 1264942241, 750770120, 1308334483, 1808072791, 1483343336, 4170849932, 3261787946, 1391377726, 2865208703], [1950175842, 47002310, 377036287, 1445470496, 1706137745, 2166273829, 1896680823, 2831856618, 2825133614, 3610138421]], dtype = 'int64')
# basis = [[2,0,1], [0,2,0], [1,0,2]]
# basis = [[3462528728, 444363041, 953498107, 3019162, 293072558, 454157756, 1950819278, 2174448309, 2956307025, 2050440410], [1676991435, 2190045933, 3360605833, 3782131535, 1076691369, 1286267085, 3124954673, 1356441824, 1675223571, 3617515077], [3467837865, 2727897666, 2512785299, 3013662465, 108820214, 2883385007, 1068246967, 798890469, 2874893319, 279879480], [3036791821, 956443849, 199347373, 1643597530, 1622084324, 471638746, 1055553070, 2020155791, 1772672903, 179014643], [3726309520, 3416039174, 816778328, 4178510478, 1052560161, 2365858231, 3810589638, 1478643591, 3537405930, 1196123656], [2101281032, 731853286, 35290828, 1463099681, 62123405, 1905949073, 3166373256, 2899371508, 674265594, 3891491225], [2924313520, 4018395377, 1444789986, 2465339761, 1288445553, 3964167277, 3710424026, 2819673876, 103363586, 3597674584], [3237227071, 578279073, 119471827, 325475776, 1510452858, 210663454, 2633248087, 2140229824, 153673582, 2811110910], [1566590271, 1264942241, 750770120, 1308334483, 1808072791, 1483343336, 4170849932, 3261787946, 1391377726, 2865208703], [1950175842, 47002310, 377036287, 1445470496, 1706137745, 2166273829, 1896680823, 2831856618, 2825133614, 3610138421]]
# basis = [[1,0,0,0,0],[0,1,0,0,0], [0,0,1,0,0],[0,0,0,1,0],[0,0,0,0,1]]
# basis = [[1,1,0], [-1,2,1], [3,-1,2]]
# basis =  [[18704, 43309], [64726, 54865]]
# basis = [[2709894480, 2051880200], [3409234876, 724643694]]
# print(solve_svp(basis))
# print(LLL(basis))
# print([np.dot(curr_scalars[i], basis[i]) for i in range(1)])
# print(round(1.5))
# print(round(-1.5))
# f = open("tests.txt", "r")
# # print(f.read())
# str_to_change = f.read()
# print(type(str_to_change))
# print(str_to_change)
# str_to_change = str_to_change.replace(",", "")
# print(str_to_change)
# f.close()
# f=  open("tests.txt", "w")
# f.write(str_to_change)
# f.close()
# ans = [-1151978862, -848083287, 785391780, 535516271, 223743428, -312571982, 1155548612, -813369737, -412398887, 129069456]
ans = [-88940, -183620, 177543, -44959, -338781, -64553, -176600, 404720, 72185, -125052, -177767, -296195, 210045, 475902]
print(math.sqrt(np.dot(ans, ans)))