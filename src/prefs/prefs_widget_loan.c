/* *******************************************************************************/
/*                                 GRISBI                                        */
/*              Programme de gestion financière personnelle                      */
/*                              license : GPLv2                                  */
/*                                                                               */
/*     Copyright (C)    2000-2008 Cédric Auger (cedric@grisbi.org)               */
/*                      2003-2008 Benjamin Drieu (bdrieu@april.org)              */
/*          2008-2017 Pierre Biava (grisbi@pierre.biava.name)                    */
/*          http://www.grisbi.org                                                */
/*                                                                               */
/*     This program is free software; you can redistribute it and/or modify      */
/*     it under the terms of the GNU General Public License as published by      */
/*     the Free Software Foundation; either version 2 of the License, or         */
/*     (at your option) any later version.                                       */
/*                                                                               */
/*     This program is distributed in the hope that it will be useful,           */
/*     but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*     GNU General Public License for more details.                              */
/*                                                                               */
/*     You should have received a copy of the GNU General Public License         */
/*     along with this program; if not, write to the Free Software               */
/*     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
/*                                                                               */
/* *******************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <glib/gstdio.h>
#include <glib/gi18n.h>

/*START_INCLUDE*/
#include "prefs_widget_loan.h"
#include "bet_data_finance.h"
#include "gsb_calendar_entry.h"
#include "gsb_data_account.h"
#include "structures.h"
#include "utils_prefs.h"
#include "erreur.h"

/*END_INCLUDE*/

/*START_EXTERN*/
/*END_EXTERN*/

typedef struct _PrefsWidgetLoanPrivate   PrefsWidgetLoanPrivate;

struct _PrefsWidgetLoanPrivate
{
	GtkWidget *			vbox_loan_data;

	GtkWidget *			grid_loan_data;
	GtkWidget *			spinbutton_rate_interest;
    GtkWidget *			checkbutton_amount_first_is_different;
	//~ GtkWidget *			eventbox_;
    //~ GtkWidget *         spinbutton_n;
    //~ GtkWidget *         filechooserbutton_;

};

G_DEFINE_TYPE_WITH_PRIVATE (PrefsWidgetLoan, prefs_widget_loan, GTK_TYPE_BOX)

/******************************************************************************/
/* Private functions                                                          */
/******************************************************************************/
/**
 * Création du widget de gestion des prêts
 *
 * \param loan
 *
 * \return
 */
static void prefs_widget_loan_setup_widget (PrefsWidgetLoan *loan)
{
	GtkWidget *widget;
	PrefsWidgetLoanPrivate *priv;

	devel_debug (NULL);

	priv = prefs_widget_loan_get_instance_private (loan);

	/* Date of first Repayment */
	widget = gsb_calendar_entry_new (FALSE);
    //~ g_object_set_data ( G_OBJECT ( parent ), "bet_config_start_date", widget );
	gtk_grid_attach (GTK_GRID (priv->grid_loan_data), widget, 1, 4, 1, 1);

	/* Annuel rate interest */
	gtk_spin_button_set_digits (GTK_SPIN_BUTTON (priv->spinbutton_rate_interest), BET_TAUX_DIGITS);

    /* set the variables for account */
    //~ gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (priv->checkbutton_),
								  //~ conf.);
	/* set spinbutton value */
	//~ gtk_spin_button_set_value (GTK_SPIN_BUTTON (priv->spinbutton_),
							   //~ conf.);

    /* Connect signal */
    //~ g_signal_connect (priv->eventbox_,
					  //~ "button-press-event",
					  //~ G_CALLBACK (utils_prefs_widget_eventbox_clicked),
					  //~ priv->checkbutton_);

    //~ g_signal_connect (priv->checkbutton_,
					  //~ "toggled",
					  //~ G_CALLBACK (utils_prefs_widget_checkbutton_changed),
					  //~ &conf.);

    /* callback for spinbutton_ */
    //~ g_object_set_data (G_OBJECT (priv->spinbutton_),
                       //~ "button", priv->checkbutton_);
	//~ g_object_set_data (G_OBJECT (priv->checkbutton_),
                       //~ "spinbutton", priv->spinbutton_);

    //~ g_signal_connect (priv->spinbutton_,
					  //~ "value-changed",
					  //~ G_CALLBACK (utils_prefs_spinbutton_changed),
					  //~ &conf.);

    /* connect the signal for filechooserbutton_backup */
    //~ g_signal_connect (G_OBJECT (priv->filechooserbutton_backup),
                      //~ "selection-changed",
                      //~ G_CALLBACK (utils_prefs_widget_dir_chosen),
                      //~ "backup_path");
	gtk_widget_show_all (priv->vbox_loan_data);
}

/******************************************************************************/
/* Fonctions propres à l'initialisation des fenêtres                          */
/******************************************************************************/
static void prefs_widget_loan_init (PrefsWidgetLoan *loan)
{
	gtk_widget_init_template (GTK_WIDGET (loan));

	prefs_widget_loan_setup_widget (loan);
}

static void prefs_widget_loan_dispose (GObject *object)
{
	G_OBJECT_CLASS (prefs_widget_loan_parent_class)->dispose (object);
}

static void prefs_widget_loan_class_init (PrefsWidgetLoanClass *klass)
{
	G_OBJECT_CLASS (klass)->dispose = prefs_widget_loan_dispose;

	gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
												 "/org/gtk/grisbi/ui/prefs_widget_loan.ui");

	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), PrefsWidgetLoan, vbox_loan_data);
	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), PrefsWidgetLoan, grid_loan_data);
	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), PrefsWidgetLoan, spinbutton_rate_interest);
	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), PrefsWidgetLoan, checkbutton_amount_first_is_different);
	//~ gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), PrefsWidgetLoan, checkbutton_);
	//~ gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), PrefsWidgetLoan, eventbox_);
	//~ gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), PrefsWidgetLoan, spinbutton_);
	//~ gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), PrefsWidgetLoan, filechooserbutton_);
}

/******************************************************************************/
/* Public functions                                                           */
/******************************************************************************/
/**
 *
 *
 * \param
 *
 * \return
 **/
PrefsWidgetLoan *prefs_widget_loan_new (void)
{
  return g_object_new (PREFS_WIDGET_LOAN_TYPE, NULL);
}

/**
 * initialise données du loan
 *
 * \param	loan widget
 * \param	account number
 * \param	version number of loan
 *
 * \return
 **/
void prefs_widget_loan_initialise_widget (PrefsWidgetLoan loan,
											 gint account_number,
											 gint version_number)
{
    GtkWidget *widget;
    GtkWidget *button = NULL;
    gchar *code_devise;
    gint devise;
    gint nbre_ans;
    gdouble taux;

    /* devel_debug (NULL); */
    //~ parent = g_object_get_data ( G_OBJECT ( notebook ), "Data_for_credit" );
    /* get devise */
    devise = gsb_data_account_get_currency ( account_number );
    code_devise = gsb_data_currency_get_code_or_isocode ( devise );

    /* set devise labels */
    //~ widget = g_object_get_data ( G_OBJECT ( parent ), "bet_config_loan_devise" );
    //~ gtk_label_set_label ( GTK_LABEL ( widget ), code_devise );

    //~ widget = g_object_get_data ( G_OBJECT ( parent ), "bet_config_frais_devise" );
    //~ gtk_label_set_label ( GTK_LABEL ( widget ), code_devise );

    //~ /* set loan */
    //~ widget = g_object_get_data ( G_OBJECT ( parent ), "bet_config_loan" );
    //~ gtk_entry_set_text ( GTK_ENTRY ( widget ),
                        //~ utils_real_get_string_with_currency (
                        //~ gsb_real_double_to_real (
                        //~ gsb_data_account_get_bet_finance_loan ( account_number ) ),
                        //~ devise, FALSE ) );
    //~ g_signal_connect ( G_OBJECT ( widget ),
                        //~ "changed",
                        //~ G_CALLBACK ( bet_config_account_amount_entry_changed ),
                        //~ parent );
    //~ g_signal_connect_after ( G_OBJECT (widget),
                        //~ "focus-out-event",
                        //~ G_CALLBACK ( bet_config_account_entry_lose_focus ),
                        //~ parent );

    //~ /* set duration */
    //~ nbre_ans = gsb_data_account_get_bet_months ( account_number ) / 12;
    //~ button = g_object_get_data ( G_OBJECT ( parent ), "bet_config_duree" );
    //~ gtk_spin_button_set_value ( GTK_SPIN_BUTTON ( button ), nbre_ans );
    //~ widget = g_object_get_data ( G_OBJECT ( parent ), "bet_config_unit_duree" );
    //~ if ( nbre_ans == 1 )
        //~ gtk_label_set_label ( GTK_LABEL ( widget ), _(" year ") );
    //~ else
        //~ gtk_label_set_label ( GTK_LABEL ( widget ), _(" years ") );
    //~ g_signal_connect ( button,
                        //~ "value-changed",
                        //~ G_CALLBACK ( bet_config_finance_apply_clicked ),
                        //~ parent );

    //~ /* set start_date */
    //~ widget = g_object_get_data ( G_OBJECT ( parent ), "bet_config_start_date" );
    //~ gsb_calendar_entry_set_date ( widget,
                        //~ gsb_data_account_get_bet_start_date ( account_number ) );
    //~ g_signal_connect_after ( G_OBJECT (widget),
                        //~ "focus-out-event",
                        //~ G_CALLBACK ( bet_config_account_date_entry_lose_focus ),
                        //~ parent );
    //~ g_signal_connect ( G_OBJECT ( widget ),
                       //~ "key-press-event",
                       //~ G_CALLBACK ( bet_config_account_date_key_press_event ),
                       //~ parent );

    //~ /* set taux */
    //~ button = g_object_get_data ( G_OBJECT ( parent ), "bet_config_taux" );
    //~ taux = gsb_data_account_get_bet_finance_taux_annuel ( account_number );
    //~ gtk_spin_button_set_value ( GTK_SPIN_BUTTON ( button ), taux );
    //~ g_signal_connect ( button,
                        //~ "value-changed",
                        //~ G_CALLBACK ( bet_config_finance_apply_clicked ),
                        //~ parent );

    //~ /* set frais */
    //~ widget = g_object_get_data ( G_OBJECT ( parent ), "bet_config_montant_frais" );
    //~ gtk_entry_set_text ( GTK_ENTRY ( widget ),
                        //~ utils_real_get_string_with_currency (
                        //~ gsb_real_double_to_real (
                        //~ gsb_data_account_get_bet_finance_frais ( account_number ) ),
                        //~ devise, FALSE ) );
    //~ g_signal_connect ( G_OBJECT ( widget ),
                        //~ "changed",
                        //~ G_CALLBACK ( bet_config_account_amount_entry_changed ),
                        //~ parent );
    //~ g_signal_connect_after ( G_OBJECT (widget),
                        //~ "focus-out-event",
                        //~ G_CALLBACK ( bet_config_account_entry_lose_focus ),
                        //~ parent );

    //~ /* set type taux */
    //~ button = g_object_get_data ( G_OBJECT ( parent ), "bet_config_type_taux" );
    //~ gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON ( button ),
                        //~ gsb_data_account_get_bet_finance_type_taux ( account_number ) );
    //~ g_signal_connect ( G_OBJECT ( button ),
                        //~ "button-release-event",
                        //~ G_CALLBACK ( bet_config_account_type_taux_changed ),
                        //~ parent );

    //~ button = g_object_get_data ( G_OBJECT ( parent ), "bet_config_type_taux_1" );
    //~ g_signal_connect ( G_OBJECT ( button ),
                        //~ "button-release-event",
                        //~ G_CALLBACK ( bet_config_account_type_taux_changed ),
                        //~ parent );
}

/**
 *
 *
 * \param
 *
 * \return
 **/
/* Local Variables: */
/* c-basic-offset: 4 */
/* End: */

