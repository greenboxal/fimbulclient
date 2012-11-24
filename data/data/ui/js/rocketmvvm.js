/* RocketMVVM */

(function(exports, $) {
	var rocketmvvm = {};
	
	/* Utils */
	function extendObject(target, extension) {
		for (var key in extension) {
			target[key] = extension[key];
		}
	}

	/* Observable Base */
	var abstractObservable = {
		_internalTrigger: function(newValue) {
			this._observers.forEach(function(observer) {
				observer.call(this);
			}, this);
		},
		addObserver: function(observer) {
			this._observers.push(observer);
		}
	};
	function createObservable(target) {
		extendObject(target, abstractObservable);
		target._observers = [];
	}
	
	/* Version */
	rocketmvvm.version = '1.0.0';
	
	/* Observable Object */
	rocketmvvm.observable = function(value) {
		function observable() {
			return observable._value;
		}
		
		createObservable(observable);
		observable._value = value;
		observable.set = function(value, trigger) {
			observable._value = value;
			
			if (trigger == true || trigger == undefined)
				this._internalTrigger(observable._value);
		}
		
		return observable;
	}
	
	/* Computable Observable */
	rocketmvvm.computed = function(evaluator, target, dependencies) {
		if (dependencies == undefined)
			dependencies = [];
	
		function computed() {
			return computed._value;
		}
		
		createObservable(computed);
		computed._value = evaluator.call(target);
		
		dependencies.forEach(function(dependency) {
			target[dependency].addObserver(function() {
				computed._value = evaluator.call(target);
				computed._internalTrigger(computed._value);
			});
		}, computed);
		
		return computed;
	}
	
	/* ViewModel */
	rocketmvvm.ViewModel = function(template) {
		this.model = new template();
		this.bindings = {};
		
		/* Apply Bindings */
		this.applyBindings = function(target) {
			var self = this;
			var $this = $(target);
			var bindingstr = $this.data().bind;
		
			if (bindingstr != undefined) {
				bindingstr.split(',', 2).forEach(function(binding) {
					if (binding == '')
						return;
					
					var params = binding.split(':');
					var type = jQuery.trim(params[0]);
					var args = undefined;
				
					if (params.length > 1)
						args = jQuery.trim(params[1]);
						
					if (this.bindings[type] != undefined) {
						if (this.bindings[type].init != undefined) {
							this.bindings[type].init.call(this, $this, this.model[args]);
						}
						
						if (this.bindings[type].update != undefined) {
							if (this.model[args] != undefined && this.model[args].addObserver != undefined) {
								this.model[args].addObserver(function() {
									self.bindings[type].update.call(self, $this, this);
								});
								self.bindings[type].update.call(self, $this, this.model[args]);
							}
						}
					}
				}, this);
			}
			
			jQuery.each($this.children(), function(i, element) {
				self.applyBindings(element);
			});
		}
		
		/* Update */
		this.update = function(data) {
			for (var key in data) {
				if (this.model[key] != undefined && this.model[key].set != undefined) {
					this.model[key].set(data[key]);
				}
			}
		}
		
		/* Bindings */
		this.bindings.text = {
			update: function($this, bind) {
				$this.text(bind());
			}
		}
		this.bindings.html = {
			update: function($this, bind) {
				$this.html(bind());
			}
		}
		this.bindings.css = {
			update: function($this, bind) {
				$this.css(bind());
			}
		}
		this.bindings.value = {
			init: function($this, bind) {
				if (bind.set != undefined) {
					$this.change(function() {
						bind.set($this.val());
					});
				}
			},
			update: function($this, bind) {
				$this.val(bind());
			}
		}
		this.bindings.checked = {
			init: function($this, bind) {
				if (bind.set != undefined) {
					$this.change(function() {
						bind.set($this.is(':checked'));
					});
				}
			},
			update: function($this, bind) {
				if (bind()) {
					$this.attr('checked', 'checked');
				} else {
					$this.removeAttr('checked');
				}
			}
		}
		this.bindings.enable = {
			update: function($this, bind) {
				if (bind()) {
					$this.removeAttr('disabled');
				} else {
					$this.attr('disabled', 'disabled');
				}
			}
		}
		this.bindings.disable = {
			update: function($this, bind) {
				if (bind()) {
					$this.attr('disabled', 'disabled');
				} else {
					$this.removeAttr('disabled');
				}
			}
		}
		this.bindings.click = {
			init: function($this, bind) {
				var self = this;
				if (jQuery.isFunction(bind)) {
					$this.click(function() {
						bind.call(self.model);
					});
				} else if (bind.execute != undefined && bind.execute.call != undefined) {
					$this.click(function() {
						bind.execute.call(self.model);
					});
				}
			}
		}
		this.bindings.options = {
			update: function($this, bind) {
				$this.empty();
				bind.each(function(key, value) {
					$this.append($('<option></option>').val(key).text(value));
				});
			}
		}
		this.bindings.selectedOptions = {
			/* TODO: IMPLEMENT IM LAZY RIGHT NOW */
		}
	}
	
	exports.mvvm = rocketmvvm;
})(window, jQuery);
