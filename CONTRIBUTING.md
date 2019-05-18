# How to Contribute

Two simple steps:

* Fork this repository to your github account
* Clone it to your machine

Now you can start working on the project.

## Open Issues

Check out `bug_report.md` and `feature_request.md` templates in the `.github` folder to know how to make a bug report or feature request.

## Here are things you could work on:

> You can check out the TODO file to see what I've planned on doing, in case you want to work on any of those topics

1. New features, new functions, new stuff!
    * Before adding anything new, please, first create an issue for it to be discussed;
    * Write tests to anything new you make.
2. Writing tests to try to reproduce those pesky bugs!
    * Anything you might think that have not been tested yet.
3. Writing the documentation.
    * Don't write any documentation in the source files, only in `Documentation.md`.
4. Writing cool examples.
5. Writing benchmarks

## But to do so you must:

1. Maintain the code quality
    * 4 space indentation;
    * Preferably don't clump up the code, try to make it well spaced by using empty lines;
    * Opening braces go underneath;
    * Give some sense to the variable names.
2. Maintain the structure of the project
    * Separate files into folders when possible;
    * Keep the `src` folder clean from anything that is not source code;
    * Create a new folder for each example with a meaningful name if it has more than one file;
3. If something looks complicated, explain it as well as possible
4. All source files (`.c`, `.h`, etc) must contain a header with the following template:
    * Complete file name.
    * First commit date in `dd/mm/yyyy`.
    * Authors name with (optional) link to github page.

```
/**
 * file_name.c
 *
 * Creation Date: dd/mm/yyyy
 *
 * Authors:
 * Author 1 Name (https://github.com/AuthorGithubPage)
 * Author 2 Name
 *
 */
```

**Make sure your name is listed in the AUTHORS file**
