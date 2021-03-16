#!/home/mzamith/Apps/anaconda3/bin/python

#https://emredjan.github.io/blog/2017/07/19/plotting-distributions/?source=post_page-----c5ebaafdeedd----------------------
import scipy.stats as ss
import numpy as np
import matplotlib.pyplot as plt
import math

def plot_beta(x_range, a, b, cdf=False, **kwargs):
    '''
    Plots the f distribution function for a given x range, a and b
    If mu and sigma are not provided, standard beta is plotted
    If cdf=True cumulative distribution is plotted
    Passes any keyword arguments to matplotlib plot function
    '''
    x = x_range
    if cdf:
        y = ss.beta.cdf(x, a, b)
    else:
        y = ss.beta.pdf(x, a, b)
    plt.plot(x, y, **kwargs)

def plot_beta_ac(x_range, a, b,  **kwargs):
    
    x = x_range
    y = ss.beta.pdf(x, a, b)

    for i in range(0, len(x)):
       v = math.floor(x[i] * 5 + 0.5);
      #v = v * 5
       x[i] = v
  #print(x[i])
    plt.xlim([0, 5])
    plt.plot(x, y,  **kwargs)
    

alpha = 6
beta = 1
#plot_beta_ac(np.linspace(0, 1, 5000), 2, 1, color='red', lw=2, ls='-', alpha=0.5, label='')
#plot_beta_ac(np.linspace(0, 1, 5000), 3, 2, color='blue', lw=2, ls='-', alpha=0.5, label='')
#plot_beta(x, alpha, beta, cdf=True, color='blue', lw=2, ls='-', alpha=0.5, label='cdf')
#plt.legend();
plot_beta(np.linspace(0, 1, 5000), alpha, beta, cdf=True, color='blue', lw=2, ls='-', alpha=0.5, label='cdf')
plot_beta(np.linspace(0, 1, 5000), alpha, beta, cdf=False, color='red', lw=2, ls='-', alpha=0.5, label='cdf')
plt.legend();

plt.show()
#plt.savefig('books_read.png')
