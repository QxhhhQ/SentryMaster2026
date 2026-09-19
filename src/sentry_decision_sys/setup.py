import os

from setuptools import find_packages, setup
from glob import glob

package_name = 'sentry_decision_sys'


def package_files(directory):
    """Install a resource tree while preserving its relative directories."""
    data_files = []
    for path, directories, filenames in os.walk(directory):
        directories[:] = [name for name in directories if name != '__pycache__']
        filenames = [
            name for name in filenames
            if not name.endswith('.py') and not name.startswith('.')
        ]
        if not filenames:
            continue
        install_path = os.path.join('share', package_name, path)
        data_files.append(
            (install_path, [os.path.join(path, filename) for filename in filenames])
        )
    return data_files

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/launch', glob('launch/*.py')),
    ] + package_files('simulator'),
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='xhhh',
    maintainer_email='3431835850@qq.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'sentry_decision_node = sentry_decision_sys.decision_node:main',
        ],
    },
)
