from setuptools import setup
from os.path import join as pjoin
from os.path import dirname

# Utility function to read the README file.
# Used for the long_description.  It's nice, because now 1) we have a top level
# README file and 2) it's easier to type in the README file than to put a raw
# string in below ...
def read(fname):
    return open(pjoin(dirname(__file__), fname)).read()

setup(
    name='or-tools',
    version='1.VVVV',
    packages=['ortools'],
    install_requires = ['google-apputils >= 0.4'],
    dependency_links = ['http://google-apputils-python.googlecode.com/files/'],
    data_files=[(pjoin('ortools', 'constraint_solver'),
                 [pjoin('ortools', 'constraint_solver', '_pywrapcp.dll'),
                  pjoin('ortools', 'constraint_solver', '_pywraprouting.dll')]),
                (pjoin('ortools', 'linear_solver'),
                 [pjoin('ortools', 'linear_solver', '_pywraplp.dll'), ]),
                (pjoin('ortools', 'graph'),
                 [pjoin('ortools', 'graph', '_pywrapgraph.dll'), ]),
                (pjoin('ortools', 'algorithms'),
                 [pjoin('ortools', 'algorithms', '_pywrapknapsack_solver.dll'), ]),
               ],
    license='Apache 2.0',
    author = 'Google Inc',
    author_email = 'lperron@google.com',
    description = 'Google OR-Tools python libraries and modules',
    keywords = ('operations research, constraint programming, ' +
                'linear programming,' + 'flow algorithms,' +
                'python'),
    url = 'http://code.google.com/p/or-tools/',
    download_url = 'https://drive.google.com/#folders/0B2yUSpEp04BNdEU4QW5US1hvTzg',
    classifiers = [
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: Apache Software License',
        'Operating System :: MacOS :: MacOS X',
        'Operating System :: Microsoft :: Windows',
        'Operating System :: POSIX :: Linux',
        'Programming Language :: Python',
        'Topic :: Office/Business :: Scheduling',
        'Topic :: Scientific/Engineering',
        'Topic :: Scientific/Engineering :: Mathematics',
        'Topic :: Software Development :: Libraries :: Python Modules'],
    long_description = read('README.txt'),
)
