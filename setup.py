from setuptools import setup, find_packages

setup(
    name="RaspberryRacer",
    version="0.1",
    
    description="Raspberry Racer",
    author="Diez B. Roggisch",
    author_email="deets@web.de",
    entry_points= {
        'console_scripts' : [
            'rracer = rracer.main:main',
            ]},
    install_requires = [
    ],
    zip_safe=True,
    packages=find_packages(),
    classifiers = [
        'Development Status :: 3 - Alpha',
        'Operating System :: OS Independent',
        'Programming Language :: Python',
    ],
    )
    
