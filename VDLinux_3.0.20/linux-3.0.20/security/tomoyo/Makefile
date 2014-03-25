obj-y = audit.o common.o condition.o domain.o environ.o file.o gc.o group.o load_policy.o memory.o mount.o network.o realpath.o securityfs_if.o tomoyo.o util.o

$(obj)/policy/profile.conf:
	@mkdir -p $(obj)/policy/
	@echo Creating a default policy/profile.conf
	@echo PROFILE_VERSION=20110903 >> $@
	@echo 0-COMMENT=-----Disabled Mode----- >> $@
	@echo 0-PREFERENCE={ max_audit_log=1024 max_learning_entry=2048 enforcing_penalty=0 } >> $@
	@echo 0-CONFIG={ mode=disabled grant_log=no reject_log=yes } >> $@
	@echo 1-COMMENT=-----Learning Mode----- >> $@
	@echo 1-PREFERENCE={ max_audit_log=1024 max_learning_entry=2048 enforcing_penalty=0 } >> $@
	@echo 1-CONFIG={ mode=learning grant_log=no reject_log=yes } >> $@
	@echo 2-COMMENT=-----Permissive Mode----- >> $@
	@echo 2-PREFERENCE={ max_audit_log=1024 max_learning_entry=2048 enforcing_penalty=0 } >> $@
	@echo 2-CONFIG={ mode=permissive grant_log=no reject_log=yes } >> $@
	@echo 3-COMMENT=-----Enforcing Mode----- >> $@
	@echo 3-PREFERENCE={ max_audit_log=1024 max_learning_entry=2048 enforcing_penalty=0 } >> $@
	@echo 3-CONFIG={ mode=enforcing grant_log=no reject_log=yes } >> $@
	

$(obj)/policy/exception_policy.conf:
	@mkdir -p $(obj)/policy/
	@echo Creating a default policy/exception_policy.conf
	@echo initialize_domain /sbin/modprobe from any >> $@
	@echo initialize_domain /sbin/hotplug from any >> $@

$(obj)/policy/domain_policy.conf:
	@mkdir -p $(obj)/policy/
	@echo Creating an empty policy/domain_policy.conf
	@touch $@

$(obj)/policy/manager.conf:
	@mkdir -p $(obj)/policy/
	@echo Creating an empty policy/manager.conf
	@touch $@

$(obj)/policy/stat.conf:
	@mkdir -p $(obj)/policy/
	@echo Creating an empty policy/stat.conf
	@touch $@

$(obj)/builtin-policy.h: $(obj)/policy/profile.conf $(obj)/policy/exception_policy.conf $(obj)/policy/domain_policy.conf $(obj)/policy/manager.conf $(obj)/policy/stat.conf
	@echo Generating built-in policy for TOMOYO 2.5.x.
	@echo "static char tomoyo_builtin_profile[] __initdata =" > $@.tmp
	@sed -e 's/\\/\\\\/g' -e 's/\"/\\"/g' -e 's/\(.*\)/"\1\\n"/' < $(obj)/policy/profile.conf >> $@.tmp
	@echo "\"\";" >> $@.tmp
	@echo "static char tomoyo_builtin_exception_policy[] __initdata =" >> $@.tmp
	@sed -e 's/\\/\\\\/g' -e 's/\"/\\"/g' -e 's/\(.*\)/"\1\\n"/' < $(obj)/policy/exception_policy.conf >> $@.tmp
	@echo "\"\";" >> $@.tmp
	@echo "static char tomoyo_builtin_domain_policy[] __initdata =" >> $@.tmp
	@sed -e 's/\\/\\\\/g' -e 's/\"/\\"/g' -e 's/\(.*\)/"\1\\n"/' < $(obj)/policy/domain_policy.conf >> $@.tmp
	@echo "\"\";" >> $@.tmp
	@echo "#ifdef CONFIG_SECURITY_TOMOYO_MANAGER" >> $@.tmp
	@echo "static char tomoyo_builtin_manager[] __initdata =" >> $@.tmp
	@sed -e 's/\\/\\\\/g' -e 's/\"/\\"/g' -e 's/\(.*\)/"\1\\n"/' < $(obj)/policy/manager.conf >> $@.tmp
	@echo "\"\";" >> $@.tmp
	@echo "#endif" >> $@.tmp
	@echo "static char tomoyo_builtin_stat[] __initdata =" >> $@.tmp
	@sed -e 's/\\/\\\\/g' -e 's/\"/\\"/g' -e 's/\(.*\)/"\1\\n"/' < $(obj)/policy/stat.conf >> $@.tmp
	@echo "\"\";" >> $@.tmp
	@mv $@.tmp $@

$(obj)/common.o: $(obj)/builtin-policy.h
