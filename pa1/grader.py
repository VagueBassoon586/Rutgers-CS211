#!/usr/bin/env python3
import autograde
import os, os.path

assignment_name = 'PA1'
release='1'

autograde.Test.time_limit = 60

class ErrorTests(autograde.StringTests):
    def test(self, *args, **kws):
        return super().test(ref_code=1, *args, **kws)


assignment = autograde.MultiProject(
    autograde.StringTests.Project('rot13', weight=0.5),
    autograde.StringTests.Project('anagram', weight=1),
    autograde.Project('balance',
        autograde.StringTests(id='1', name='', weight=0.75),
        ErrorTests(id='2', name='', weight=0.75),
    ),
    autograde.StdinFileTests.Project('list', weight=2),
    autograde.FileTests.Project('mexp', weight=2),
    autograde.Project('bst',
        autograde.StdinFileTests(id='1', weight=1.5),
        autograde.StdinFileTests(id='2', weight=1.5),
    ),
)


if __name__ == '__main__':
    os.environ['LC_ALL'] = 'en-US'
    autograde.main(assignment_name, assignment, release)
