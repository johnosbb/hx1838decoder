# How to Submit an Arduino Library

- First take a look at the [requirements](https://github.com/arduino/library-registry/blob/main/FAQ.md#submission-requirements) for admission into the Arduino Library Manager index. Each submission will be checked for compliance before being accepted.
- Click the following link: [https://github.com/arduino/library-registry/fork](https://github.com/arduino/library-registry/fork). The "Create a new fork" page will open.
- Click the Create fork button in the "Create a new fork" page. A "Forking arduino/library-registry" page will open while the fork is in the process of being created.
- Wait for the "Forking" process to finish. The home page of your fork of the library-registry repository will open.
- Click on the file repositories.txt under the list of files you see in that page. The "library-registry/repositories.txt" page will open.
- Click the pencil icon ("Edit this file") at the right side of the toolbar in the "library-registry/repositories.txt" page. The repositories.txt file will open in the online text editor.
- Add the library repository's URL to the list (it doesn't matter where in the list). This should be the URL of the repository home page. For example: [https://github.com/johnosbb/hx1838decoder](https://github.com/johnosbb/hx1838decoder)
- Click the Commit changes... button located near the top right corner of the page. The "Commit changes" dialog will open.
- Click the Commit changes button in the "Commit changes" dialog. The "library-registry/repositories.txt" page will open.
- Click the "library-registry" link at the top of the "library-registry/repositories.txt" page. The home page of your fork of the library-registry repository will open.
- You should see a banner on the page that says: This branch is 1 commit ahead of arduino:main.
- Click the "Contribute" link near the right side of that banner. A menu will open.
- Click the Open pull request button in the menu. The "Open a pull request" page will open.
- In the "Open a pull request" window that opens, click the Create pull request button.


The library will be automatically checked for compliance as soon as the pull request is submitted. If no problems were found, the pull request will be immediately merged and the library will be available for installation via Library Manager within a day's time.

If any problems are found, a bot will comment on the pull request to tell you what is wrong. The problem may be either with your pull request or with the library.