.SUFFFIXES: .inx
STAMP=stampinf
.inx{$(OBJ_PATH)\$(0)}.inf:
	copy $(@B).inx $@
	$(STAMP) -f $@ -a $(_BUILDARCH) -u $(KMDF_VERSION).$(KMDF_MINOR_VERSION).0