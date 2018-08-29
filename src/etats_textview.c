/* ************************************************************************** */
/*                                                                            */
/*     Copyright (C)    2000-2008 Cédric Auger (cedric@grisbi.org)            */
/*          2004-2008 Benjamin Drieu (bdrieu@april.org)                       */
/*          2008-2018 Pierre Biava (grisbi@pierre.biava.name)                 */
/*          http://www.grisbi.org                                             */
/*                                                                            */
/*  This program is free software; you can redistribute it and/or modify      */
/*  it under the terms of the GNU General Public License as published by      */
/*  the Free Software Foundation; either version 2 of the License, or         */
/*  (at your option) any later version.                                       */
/*                                                                            */
/*  This program is distributed in the hope that it will be useful,           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*  GNU General Public License for more details.                              */
/*                                                                            */
/*  You should have received a copy of the GNU General Public License         */
/*  along with this program; if not, write to the Free Software               */
/*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
/*                                                                            */
/* ************************************************************************** */

/**
 * \file etats_textview.c
 * show the report in textview
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glib/gi18n.h>

 /*START_INCLUDE*/
#include "etats_affiche.h"
#include "etats_config.h"
#include "gsb_data_account.h"
#include "gsb_data_report.h"
#include "gsb_data_transaction.h"
#include "gsb_transactions_list.h"
#include "menu.h"
#include "navigation.h"
#include "structures.h"
#include "transaction_list.h"
#include "transaction_list_select.h"
#include "utils.h"
#include "erreur.h"
/*END_INCLUDE*/

/*START_STATIC*/
static GtkWidget *view = NULL;
static gint current_line = 0;
static gint nb_cols_titres = 0;
static guint col_width[27];
static guint col_offset[27];
static gint sum_offset = 0;
static gint nb_max_lignes = 5;
/*END_STATIC*/

/*START_GLOBAL*/
GtkWidget *table_etat = NULL;
gboolean *set_cols_titres = FALSE;
/*END__GLOBAL*/

/*START_EXTERN*/
extern GtkWidget *scrolled_window_etat;
extern gint nb_colonnes;
/*END_EXTERN*/

/******************************************************************************/
/* Private functions                                                          */
/******************************************************************************/
/**
 *
 *
 * \param
 *
 * \return
 **/
static gchar *etat_gtktable_set_colonne_name (const gchar *name,
											  guint long_name)
{
	gchar *tmp_name = NULL;
	gchar *chaine_vide;
	guint nbre_chars;
	guint nbre_espace;

	if ((nbre_chars = g_utf8_strlen (name, -1)) < long_name)
	{
		gint reste;

		nbre_espace = (long_name - nbre_chars)/2;
		reste = (long_name - nbre_chars)%2;
		//~ printf ("long_name = %d nbre_chars = %d nbre_espace = %d reste = %d\n", long_name, nbre_chars, nbre_espace, reste);
		if (reste)
			nbre_espace++;

		chaine_vide = g_strnfill (nbre_espace, ' ');
		tmp_name = g_strconcat (chaine_vide, name, chaine_vide, NULL);
		//~ printf ("long titre = %ld\n", g_utf8_strlen (tmp_name, -1));
	}
	else
	{
		tmp_name = g_strconcat (" ", name, " ", NULL);
	}
	return tmp_name;
}
/**
 *
 *
 * \param
 *
 * \return
 **/
static void etat_gtktable_set_colonnes_width (void)
{
    gint current_report_number;
	guint i = 1;

    current_report_number = gsb_gui_navigation_get_current_report ();
    if ( gsb_data_report_get_show_report_transaction_number (current_report_number))
    {
		col_width[i] = 10;
		i++;
		col_width[i] = 2;
		i++;

    }

    if ( gsb_data_report_get_show_report_date (current_report_number))
    {
		col_width[i] = 12;
		i++;
		col_width[i] = 2;
		i++;
    }

    if ( gsb_data_report_get_show_report_value_date (current_report_number))
    {
		col_width[i] = 12;
		i++;
		col_width[i] = 2;
		i++;
    }

    if ( gsb_data_report_get_show_report_financial_year (current_report_number))
    {
		col_width[i] = 6;
		i++;
		col_width[i] = 2;
		i++;
    }

    if ( gsb_data_report_get_show_report_payee (current_report_number))
    {
		col_width[i] = 30;
		i++;
		col_width[i] = 2;
		i++;
    }

    if ( gsb_data_report_get_show_report_category (current_report_number))
    {
		col_width[i] = 30;
		i++;
		col_width[i] = 2;
		i++;
    }

    if ( gsb_data_report_get_show_report_budget (current_report_number))
    {
		col_width[i] = 30;
		i++;
		col_width[i] = 2;
 		i++;
   }

    if ( gsb_data_report_get_show_report_note (current_report_number))
    {
		col_width[i] = 40;
		i++;
		col_width[i] = 2;
		i++;
    }

    if ( gsb_data_report_get_show_report_method_of_payment (current_report_number))
    {
		col_width[i] = 20;
		i++;
		col_width[i] = 2;
		i++;
    }

    if ( gsb_data_report_get_show_report_method_of_payment_content (current_report_number))
    {
		col_width[i] = 20;
		i++;
		col_width[i] = 2;
 		i++;
   }

    if ( gsb_data_report_get_show_report_voucher (current_report_number))
    {
		col_width[i] = 20;
		i++;
		col_width[i] = 2;
		i++;
    }

    if ( gsb_data_report_get_show_report_bank_references (current_report_number))
    {
		col_width[i] = 20;
		i++;
		col_width[i] = 2;
		i++;
    }

    if ( gsb_data_report_get_show_report_marked (current_report_number))
    {
		col_width[i] = 3;
		i++;
		col_width[i] = 2;
		i++;
    }
	/* set amount */
	col_width[i] = 12;

	nb_cols_titres = i;
	//~ printf ("nb_cols_titres = %d\n", i);
}

/**
 * Crée un textview pour afficher le résultat du rapport.
 *
 * \param GtkTextBuffer
 *
 * \return
 **/
static void etats_textview_create_tags (GtkTextBuffer *buffer)
{
  gtk_text_buffer_create_tag (buffer, "italic",
                              "style", PANGO_STYLE_ITALIC, NULL);

  gtk_text_buffer_create_tag (buffer, "bold",
                              "weight", PANGO_WEIGHT_BOLD, NULL);

  gtk_text_buffer_create_tag (buffer, "small",
                              "scale", PANGO_SCALE_SMALL, NULL);

  gtk_text_buffer_create_tag (buffer, "large",
                              "scale", PANGO_SCALE_LARGE, NULL);

  gtk_text_buffer_create_tag (buffer, "x_large",
                              "scale", PANGO_SCALE_X_LARGE, NULL);

  gtk_text_buffer_create_tag (buffer, "huge",
                              "scale", PANGO_SCALE_XX_LARGE, NULL);

  gtk_text_buffer_create_tag (buffer, "center",
                              "justification", GTK_JUSTIFY_CENTER, NULL);

  gtk_text_buffer_create_tag (buffer, "fill_justify",
                              "justification", GTK_JUSTIFY_FILL, NULL);

  gtk_text_buffer_create_tag (buffer, "left_justify",
                              "justification", GTK_JUSTIFY_LEFT, NULL);

  gtk_text_buffer_create_tag (buffer, "right_justify",
                              "justification", GTK_JUSTIFY_RIGHT, NULL);
}

/**
 * Crée un textview pour afficher le résultat du rapport.
 *
 * \param 			unused;
 * \param 			unused;
 *
 * \return 1
 **/
static gint etats_textview_initialise (GSList *opes_selectionnees,
									  gchar *filename)
{
	GtkTextBuffer *buffer;

    if (table_etat && GTK_IS_GRID (table_etat))
        gtk_widget_destroy (table_etat);

    /* regarder la liberation de mémoire */
    if (scrolled_window_etat && gtk_bin_get_child (GTK_BIN (scrolled_window_etat)))
        gtk_widget_destroy (gtk_bin_get_child (GTK_BIN (scrolled_window_etat)));

    /* just update screen so that the user does not see the previous report anymore
     * while we are processing the new report */
     update_gui ();

	/* set table */
	table_etat = gtk_grid_new ();
	gtk_widget_set_halign (table_etat, GTK_ALIGN_CENTER);

	/* st textview */
	view = gtk_text_view_new ();
	gtk_text_view_set_editable (GTK_TEXT_VIEW (view), FALSE);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (view), GTK_WRAP_WORD);
	gtk_text_view_set_left_margin (GTK_TEXT_VIEW (view), 10);
	gtk_text_view_set_right_margin (GTK_TEXT_VIEW (view), 10);
	gtk_widget_set_size_request (view, conf.form_expander_label_width, -1);
    gtk_grid_attach (GTK_GRID (table_etat), view, 0, 0, 1, 1);

	/* create tabulations pour les colonnes */
	etat_gtktable_set_colonnes_width ();

	/* create tags */
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
	etats_textview_create_tags (buffer);

    return 1;
}

/**
 *
 *
 * \param
 *
 * \return
 **/
static void etats_textview_set_separateurs_cols (gint x,
												 gint y)
{
	GtkTextBuffer *buffer;
	GtkTextIter iter;

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
	gtk_text_buffer_get_iter_at_line_offset (buffer, &iter, y, col_offset[x]);
	gtk_text_buffer_insert (buffer, &iter, " | ", -1);
	sum_offset += 3;
	col_offset[x+1] = sum_offset;
	//~ printf ("col_offset[%d] après separateur = %d\n", x+1, col_offset[x+1]);
}

/**
 *
 *
 * \param
 *
 * \return
 **/
static void etats_textview_set_titres_cols (gint x,
											gint y,
											const gchar *text)
{
	GtkTextBuffer *buffer;
	GtkTextIter iter;
	gchar *tmp_text;
	gint nbre_chars = 0;

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
	nbre_chars = col_width[x];
	//~ printf ("nbre_chars = %d col_offset[%d] = %d\n", nbre_chars,x, col_offset[x]);
	gtk_text_buffer_get_iter_at_line_offset (buffer, &iter, y, col_offset[x]);
	tmp_text = etat_gtktable_set_colonne_name (text, nbre_chars);
	gtk_text_buffer_insert (buffer, &iter, tmp_text, -1);
	sum_offset += g_utf8_strlen (tmp_text, -1);
	col_offset[x+1] = sum_offset;
		//~ printf ("col_offset[%d] après text = %d\n", x+1, col_offset[x+1]);
}

/**
 *
 *
 * \param
 *
 * \return a newly string
 **/
static gchar *etats_textview_get_tags (gdouble properties,
									   GtkJustification align)
{
	gchar *tags = NULL;
	gchar *str_to_free;

	switch (align)
    {
		case GTK_JUSTIFY_LEFT:
			tags = g_strdup ("left_justify");
			break;
		case GTK_JUSTIFY_CENTER:
			tags = g_strdup ("center");
			break;
		case GTK_JUSTIFY_RIGHT:
			tags = g_strdup ("right_justify");
			break;
		case GTK_JUSTIFY_FILL:
			tags = g_strdup ("fill_justify");
			break;
    }

    if (((gint) properties) & TEXT_ITALIC)
	{
		str_to_free = tags;
		tags = g_strconcat (str_to_free, ",italic", NULL);
		g_free (str_to_free);
	}
    if (((gint) properties) & TEXT_BOLD)
	{
		str_to_free = tags;
		tags = g_strconcat (str_to_free, ",bold", NULL);
		g_free (str_to_free);
	}
    if (((gint) properties) & TEXT_HUGE)
	{
		str_to_free = tags;
		tags = g_strconcat (str_to_free, ",huge", NULL);
		g_free (str_to_free);
	}
    if (((gint) properties) & TEXT_LARGE)
	{
		str_to_free = tags;
		tags = g_strconcat (str_to_free, ",large", NULL);
		g_free (str_to_free);
	}
    if (((gint) properties) & TEXT_SMALL)
	{
		str_to_free = tags;
		tags = g_strconcat (str_to_free, ",small", NULL);
		g_free (str_to_free);
	}

	return tags;
}

/**
 * cette fonction est appelée si on click sur une opé dans un état
 * elle affiche la liste des opés sur cette opé
 *
 * \param transaction number
 *
 * \return
 **/
//~ static void etats_textview_click_sur_ope_etat (gint transaction_number)
//~ {
    //~ gint archive_number;
    //~ gint account_number;

    //~ account_number = gsb_data_transaction_get_account_number (transaction_number);

    //~ /* if it's an archived transaction, open the archive */
    //~ archive_number = gsb_data_transaction_get_archive_number (transaction_number);
    //~ if (archive_number)
    //~ {
		//~ /* re-filter the tree view because if we go directly into the report
		 //~ * and the model was never filtered, we have a nice crash */
		//~ transaction_list_filter (account_number);
		//~ gsb_transactions_list_restore_archive (archive_number, FALSE);
    //~ }

    //~ if (transaction_number)
    //~ {
		//~ gint mother_transaction;

		//~ /* go on the good account */
		//~ gsb_gui_navigation_set_selection (GSB_ACCOUNT_PAGE, account_number, -1);

		//~ /* récupération de la ligne de l'opé dans la liste ; affichage de toutes les opé si nécessaire */
		//~ if (gsb_data_transaction_get_marked_transaction (transaction_number) == OPERATION_RAPPROCHEE
			 //~ && !gsb_data_account_get_r (account_number))
		//~ {
			//~ gsb_data_account_set_r (account_number, TRUE);
			//~ gsb_menu_update_view_menu (account_number);
			//~ mise_a_jour_affichage_r (TRUE);
		//~ }

		//~ /* if it's a child, open the mother */
		//~ mother_transaction = gsb_data_transaction_get_mother_transaction_number (transaction_number);
		//~ if (mother_transaction)
			//~ gsb_transactions_list_switch_expander (mother_transaction);

		//~ transaction_list_select (transaction_number);
    //~ }
//~ }

/**
 * Attach a text at given positions.
 *
 * \param text					Text to display in label
 * \param properties			Bit mask of text properties
 * \param x						Left horizontal position
 * \param x2					Right horizontal position
 * \param y						Top vertical position
 * \param y2					Bottom vertical position
 * \param align					Horizonal align of the label
 * \param transaction_number	Number of the related transaction if
 *								label is part of a transaction.  Make
 *								an hyperlink if applicable
 *
 * \return
 **/
static void etats_textview_attach_label (gchar *text,
										 gdouble properties,
										 int x,
										 int x2,
										 int y,
										 int y2,
										 GtkJustification align,
										 gint transaction_number)
{
	GtkTextBuffer *buffer;
	GtkTextIter iter;
	gchar *tags;
	gchar **tab_tags;
	gint x_dim;
	gint y_dim;

	if (y > nb_max_lignes)
		return;

	if (!text)
	{
		text = "\n";
	}

	x_dim = x2 - x;
	y_dim = y2 - y;

printf ("\tx = %d\ty = %d\tcurrent_line = %d\tx_dim = %d\ty_dim = %d\ttext = %s\n", x, y, current_line, x_dim, y_dim,text);

	/* Common data */
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
	gtk_text_buffer_get_end_iter (buffer, &iter);
	tags = etats_textview_get_tags (properties, align);
	if (x_dim == nb_colonnes)	/* c'est un titre */
	{
		tab_tags = g_strsplit (tags, ",", 3);
		gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, text, -1, tab_tags[0], tab_tags[1], tab_tags[2], NULL);
		gtk_text_buffer_insert (buffer, &iter, "\n", -1);
		g_free (tags);
		g_strfreev (tab_tags);
		current_line = y+1;
		//~ printf ("nbre lignes du buffer = %d current_line = %d\n", gtk_text_buffer_get_line_count (buffer), current_line);
	}
	else
	{
		if (set_cols_titres) 	/* on met les titres de colonnes */
		{
			etats_textview_set_titres_cols (x, y, text);
			//~ printf ("nbre lignes du buffer = %d current_line = %d\n", gtk_text_buffer_get_line_count (buffer), current_line);
		}
		else
		{
			if (current_line == y)
			{

					gtk_text_buffer_insert (buffer, &iter, text, -1);
			}
			else
			{
				gtk_text_buffer_insert (buffer, &iter, "\n", -1);
				gtk_text_buffer_insert (buffer, &iter, text, -1);
				current_line = y;
			}
		}
	}

}

/**
 * Attach an horizontal separator at given positions.
 *
 * \param x			Left horizontal position
 * \param x2		Right horizontal position
 * \param y			Top vertical position
 * \param y2		Bottom vertical position
 *
 * \return
 **/
static void etats_textview_attach_hsep (int x,
										int x2,
										int y,
										int y2)
{
	GtkTextBuffer *buffer;
	GtkTextIter iter;
	gint x_dim;
	gint y_dim;

	if (y > nb_max_lignes)
		return;

	x_dim = x2 - x;
	y_dim = y2 - y;
	printf ("h_sep\tx = %d\ty = %d\tcurrent_line = %d\tx_dim = %d\ty_dim = %d\n", x, y, current_line, x_dim, y_dim);
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
	gtk_text_buffer_get_end_iter (buffer, &iter);
	gtk_text_buffer_insert (buffer, &iter, "\n", -1);
	current_line = y+1;
	//~ printf ("nbre lignes du buffer = %d current_line = %d\n", gtk_text_buffer_get_line_count (buffer), current_line);
}

/**
 * Attach a vertical separator at given positions.
 *
 * \param x			Left horizontal position
 * \param x2		Right horizontal position
 * \param y			Top vertical position
 * \param y2		Bottom vertical position
 *
 * Martin Stromberger, 28.08.2008:
 * changed xpadding from 0 to 2 for a better printlayout.
 * xpadding=0 -> no space between separator and text in printlayout.
 *
 * \return
 **/
static void etats_textview_attach_vsep (int x,
										int x2,
										int y,
										int y2)
{
	gint x_dim;
	gint y_dim;

	if (y > nb_max_lignes)
		return;

	x_dim = x2 - x;
	y_dim = y2 - y;
	printf ("v_sep\tx = %d\ty = %d\tcurrent_line = %d\tx_dim = %d\ty_dim = %d\n", x, y, current_line, x_dim, y_dim);
	if (set_cols_titres) 	/* on met les titres de colonnes */
	{
		etats_textview_set_separateurs_cols (x, y);
	}


}

static gint etats_textview_finish (void)
{
    gtk_container_add (GTK_CONTAINER (scrolled_window_etat), table_etat);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window_etat), GTK_SHADOW_NONE);

    gtk_container_set_border_width (GTK_CONTAINER (table_etat), 6);
    gtk_widget_show_all (table_etat);

    if (gtk_bin_get_child (GTK_BIN (scrolled_window_etat)))
        gtk_widget_show (gtk_bin_get_child (GTK_BIN (scrolled_window_etat)));

    return 1;
}

/******************************************************************************/
/* Public functions                                                           */
/******************************************************************************/
struct EtatAffichage textview_affichage =
{
    etats_textview_initialise,
    etats_textview_finish,
    etats_textview_attach_hsep,
    etats_textview_attach_vsep,
    etats_textview_attach_label
};

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
