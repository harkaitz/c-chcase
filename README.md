CHCASE
======

Small program to transform from CamelCase to snake_case and vice versa.

## Help message

    Usage: chcase [-CSL][-d DICT] < TEXT

    Change the casing of the input text to CamelCase (-C), snake_case (-S) or
    lisp-case (-L). By default it prints the word splet by spaces.

    For CamelCase special words such as 'API' or 'URL' can be defined in a
    dictionary, by default "/usr/share/chcase/chcase.dict", you can place extra
    words with -d or by setting the CHCASE_DICT environment variable.

## Examples

    $ echo 'modify_id send_api_request send_html' | chcase -C
    ModifyID SendAPIRequest SendHTML
    $ echo 'SendToMyFriend SendHTML' | chcase -S
    send_to_my_friend send_html
    $ echo 'CopyFile' | chcase -L
    copy-file

## Style guide

This project follows the OpenBSD kernel source file style guide (KNF).

Read the guide [here](https://man.openbsd.org/style).

## Collaborating

For making bug reports, feature requests and donations visit
one of the following links:

1. [gemini://harkadev.com/oss/](gemini://harkadev.com/oss/)
2. [https://harkadev.com/oss/](https://harkadev.com/oss/)
