#ifndef __PREFS_WIDGET_LOAN_H__
#define __PREFS_WIDGET_LOAN_H__

#include <gtk/gtk.h>
#include <glib.h>

/*START_INCLUDE*/
//~ #include "grisbi_prefs.h"
/*END_INCLUDE*/

G_BEGIN_DECLS

#define PREFS_WIDGET_LOAN_TYPE    	(prefs_widget_loan_get_type ())
#define PREFS_WIDGET_LOAN(obj)    	(G_TYPE_CHECK_INSTANCE_CAST ((obj), PREFS_WIDGET_LOAN_TYPE, PrefsWidgetLoan))
#define PREFS_IS_WIDGET_LOAN(obj) 	(G_TYPE_CHECK_INSTANCE_TYPE((obj), PREFS_WIDGET_LOAN_TYPE))

typedef struct _PrefsWidgetLoan          PrefsWidgetLoan;
typedef struct _PrefsWidgetLoanClass     PrefsWidgetLoanClass;


struct _PrefsWidgetLoan
{
    GtkBox parent;
};

struct _PrefsWidgetLoanClass
{
    GtkBoxClass parent_class;
};

/* START_DECLARATION */
GType               	prefs_widget_loan_get_type			(void) G_GNUC_CONST;

void 					prefs_widget_loan_initialise_widget	(PrefsWidgetLoan loan,
																 gint account_number,
																 gint version_number);
PrefsWidgetLoan * 		prefs_widget_loan_new				(void);

/* END_DECLARATION */

G_END_DECLS

#endif  /* __PREFS_WIDGET_LOAN_H__ */
