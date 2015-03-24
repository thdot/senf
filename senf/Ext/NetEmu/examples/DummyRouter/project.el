;; Configuration file for cc-ide.el (Emacs C++ IDE extension, see http://g0dil.de)

 (defconst senf-c-style
  '((c-basic-offset              . 4)
    (c-backslash-column          . 98)
    (c-cleanup-list              . (empty-defun-braces 
				    defun-close-semi 
				    list-close-comma 
				    scope-operator 
				    compact-empty-funcall))
    (c-hanging-braces-alist      . ((namespace-open after)
                                    (namespace-close before after)
                                    (brace-list-open)
                                    (brace-entry-open)
                                    (substatement-open after)
                                    (block-close . c-snug-do-while)
                                    (extern-lang-open after)
                                    (inexpr-class-open after)
                                    (inexpr-class-close before)))
    (c-offsets-alist             . ((namespace-open . 0)
                                    (namespace-close . -)
                                    (innamespace . +)
                                    (statement-block-intro . +)
                                    (substatement-open . 0)
                                    (label . 0)
                                    (statement-cont . +))) ))

(c-add-style "senf" senf-c-style)

(set (make-local-variable 'ccide-file-vars)
     '(( fill-column  . 100 )
       ( comment-column . 40 )
       ( c-file-style . "senf" )
       ( indent-tabs-mode . nil )
       ( ispell-local-dictionary . "american" )
       ( compile-command . "scons -U") ))

(set (make-local-variable 'ccide-default-copyright)
     (concat "//\n"
             "             "             "             "
(set (make-local-variable 'ccide-project-name) "Carmen")

(let ((local-conf (ccide-project-search-upwards "project-local.el")))
  (if local-conf
      (load-file local-conf)))

(defun flyspell-cc-progmode-verify ()
  "Replacement for standard flyspell-generic-progmode-verify which
checks for C/C++ preproc directives. Additionally, anything after ^L
is ignored (Those are the file local variables and local words)."
  (let ((f (get-text-property (point) 'face)))
    (and (memq f flyspell-prog-text-faces)
	 (not (save-excursion 
		(beginning-of-line) 
		(looking-at "\\(//\\)?#")))
	 (not (let ((l (max (point-min) (- (point-max) 4096))))
		(and (< l (point))
		     (save-excursion (search-backward "" l t))))))))

(defun flyspell-cc-mode ()
  "Torn on `flyspell-mode` for comments and strings in C/C++ mode."
  (interactive)
  (setq flyspell-generic-check-word-p 'flyspell-cc-progmode-verify)
  (flyspell-mode 1))

;; Better set this here than in the file variables since the setting
;; is only valid if project.el is loaded ...
(set (make-local-variable 'ispell-personal-dictionary)
     (expand-file-name "senf.dict" ccide-project-root))
(flyspell-cc-mode)
