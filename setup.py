from setuptools import setup, find_packages
import os

with open('README.md') as f:
    readme = f.read()

with open('VERSION') as f:
    version = f.read().strip()

DATA_FILE_LOCATION = '/usr/share/unraid_ttriing'
setup(
    name='unraid_ttriing',
    version=version,
    packages=find_packages(),
    url='https://github.com/Alyxiana/unraid-ttriing',
    license='GPL-2.0',
    author='Alyxiana',
    author_email='chestm007@hotmail.com',
    maintainer='Alyxiana',
    maintainer_email='chestm007@hotmail.com',
    description='Python driver and daemon for Thermaltake hardware products',
    long_description=readme,
    long_description_content_type="text/markdown",
    python_requires='>=3.6',
    install_requires=[
        "pyyaml>=5.1",
        "psutil>=5.0",
        "pyusb>=1.0.0",
        "numpy>=1.16"
    ],
    entry_points={
        'console_scripts': [
            'unraid-ttriing=unraid_ttriing.daemon.main:main',
        ],
    },
    data_files=[(DATA_FILE_LOCATION, ['unraid_ttriing/assets/linux-thermaltake-rgb.service']),
                (DATA_FILE_LOCATION, ['unraid_ttriing/assets/config.yml'])],
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: System Administrators',
        'License :: OSI Approved :: GNU General Public License v2 (GPLv2)',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Programming Language :: Python :: 3.10',
        'Programming Language :: Python :: 3.11',
        'Topic :: System :: Hardware',
        'Topic :: System :: Systems Administration',
    ],
)
