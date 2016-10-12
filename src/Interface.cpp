/* created by Marleson Graf<aszdrick@gmail.com>
   and Vinicius Freitas<vinicius.mctf@grad.ufsc.br> [2016] */

#include "Interface.hpp"
#include "signals.hpp"

Interface::Interface() {
    build();
}

void Interface::build() {
    builder = gtk_builder_new_from_file("../res/view_simpl.ui");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    // list = GTK_TREE_VIEW(gtk_builder_get_object(builder, "grammar_list"));
    // grammar = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "grammar_view"));
    // input = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "input_view"));
    // info_table = GTK_TREE_VIEW(gtk_builder_get_object(builder, "results_tree"));
    // auto selection = gtk_tree_view_get_selection(list);

    // gtk_tree_selection_set_select_function(selection, signals::list_selection, nullptr, nullptr);
    g_signal_connect(window, "delete-event", G_CALLBACK(signals::close), nullptr);

    connect_buttons();
}

void Interface::connect_buttons() {
    // auto add = GTK_BUTTON(gtk_builder_get_object(builder, "add_grammar"));
    // auto update = GTK_BUTTON(gtk_builder_get_object(builder, "update_grammar"));
    // auto reset = GTK_BUTTON(gtk_builder_get_object(builder, "reset_grammar_view"));
    // auto analyse = GTK_BUTTON(gtk_builder_get_object(builder, "analyse_grammar"));
    // auto gen_parse = GTK_BUTTON(gtk_builder_get_object(builder, "generate_parse"));
    // auto parse = GTK_BUTTON(gtk_builder_get_object(builder, "analyse_input"));
    // auto open_option = gtk_builder_get_object(builder, "open_file");
    // auto save_option = gtk_builder_get_object(builder, "save_file");

    // g_signal_connect(add, "clicked", G_CALLBACK(signals::add_grammar), nullptr);
    // g_signal_connect(update, "clicked", G_CALLBACK(signals::update_grammar), nullptr);
    // g_signal_connect(reset, "clicked", G_CALLBACK(signals::reset_grammar_view), nullptr);
    // g_signal_connect(analyse, "clicked", G_CALLBACK(signals::analyse_grammar), nullptr);
    // g_signal_connect(gen_parse, "clicked", G_CALLBACK(signals::generate_parse), nullptr);
    // g_signal_connect(parse, "clicked", G_CALLBACK(signals::parse_input), nullptr);

    // g_signal_connect(open_option, "activate", G_CALLBACK(signals::open), nullptr);
    // g_signal_connect(save_option, "activate", G_CALLBACK(signals::save), nullptr);
}

void Interface::show() {
    gtk_widget_show_all(window);
}

void Interface::destroy() {
    // gtk_main_quit();
    gtk_widget_destroy(window);
}

void Interface::show_message(GtkMessageType type, GtkButtonsType buttons,
                             const std::string& title, const std::string& message,
                             const std::string& submessage) const {
    auto dialog = gtk_message_dialog_new(GTK_WINDOW(window),
        GTK_DIALOG_DESTROY_WITH_PARENT, type, buttons, message.c_str());
    
    gtk_window_set_title(GTK_WINDOW(dialog), title.c_str());
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
                                             submessage.c_str());
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// std::string Interface::get_grammar_text() const {
//     GtkTextIter begin, end;
//     auto buffer = gtk_text_view_get_buffer(grammar);
//     gtk_text_buffer_get_bounds(buffer, &begin, &end);
//     std::string text = gtk_text_buffer_get_text(buffer, &begin, &end, false);
//     return text;
// }

// std::string Interface::get_input_text() const {
//     GtkTextIter begin, end;
//     auto buffer = gtk_text_view_get_buffer(input);
//     gtk_text_buffer_get_bounds(buffer, &begin, &end);
//     std::string text = gtk_text_buffer_get_text(buffer, &begin, &end, false);
//     return text;
// }


// void Interface::select_grammar(unsigned id) {
//     auto path = gtk_tree_path_new_from_string(std::to_string(id).c_str());
//     gtk_tree_view_set_cursor(list, path, nullptr, false);
// }

// void Interface::put_grammar(const std::string& name, unsigned id) {
//     auto model = GTK_TREE_MODEL(gtk_builder_get_object(builder, "grammar_store"));
//     GtkTreeIter iter;
//     gtk_list_store_insert(GTK_LIST_STORE(model), &iter, id);
//     gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, id, 1, name.c_str(), -1);
// }

// void Interface::show_grammar(const std::string& text) {
//     auto buffer = gtk_text_view_get_buffer(grammar);
//     GtkTextIter begin, end;
//     gtk_text_buffer_get_bounds(buffer, &begin, &end);
//     //gtk_text_buffer_insert_markup(buffer, &begin, "<big>\0", -1);
//     gtk_text_buffer_set_text(buffer, text.c_str(), -1);
// //    gtk_text_buffer_insert_markup(buffer, &end, "</big>\0", -1);
// }

// void Interface::show_info_table(const std::vector<std::array<std::string, 7>>& info) {
//     GType types[7] = {G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
//                       G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING};
    
//     auto model = gtk_list_store_newv(7, types);

//     for (auto line : info) {
//         GtkTreeIter iter;
//         gtk_list_store_insert(model, &iter, -1);
//         unsigned i = 0;
//         for (auto cell : line) {
//             gtk_list_store_set(model, &iter, i++, cell.c_str(), -1);
//         }
//     }

//     gtk_tree_view_set_model(info_table, GTK_TREE_MODEL(model));
// }

// void Interface::show_ll_one_parser(const StringTable& table, const StringTable& prod)  const {
//     auto pbuilder = gtk_builder_new_from_file("parser_table.ui");
//     auto pwindow = GTK_WIDGET(gtk_builder_get_object(pbuilder, "parser_window"));
//     auto parsing_table = GTK_TREE_VIEW(gtk_builder_get_object(pbuilder, "parsing_table"));
//     auto enum_table = GTK_TREE_VIEW(gtk_builder_get_object(pbuilder, "enum_table"));
//     g_signal_connect(pwindow, "delete-event", G_CALLBACK(gtk_widget_destroy), (gpointer)pwindow);

//     GType* types = new GType[table[0].size()];

//     auto size = gtk_tree_view_get_n_columns(parsing_table);
//     for (unsigned i = 0; i < size; i++) {
//         auto column = gtk_tree_view_get_column(parsing_table, 0);
//         gtk_tree_view_remove_column(parsing_table, column);
//     }

//     for (unsigned i = 0; i < table[0].size(); i++) {
//         types[i] = G_TYPE_STRING;
//         auto renderer = gtk_cell_renderer_text_new();
//         if (i == 0) gtk_cell_renderer_set_alignment(renderer, 0.5, 0.5);
//         gtk_tree_view_insert_column_with_attributes(parsing_table, -1, table[0][i].c_str(),
//                                                    renderer, "text", i,
//                                                    nullptr);
//     }
    
//     auto model = gtk_list_store_newv(table[0].size(), types);
//     g_free(types);

//     for (unsigned i = 1; i < table.size(); i++) {
//         GtkTreeIter iter;
//         gtk_list_store_insert(model, &iter, -1);
//         unsigned j = 0;
//         for (auto s : table[i]) {
//             gtk_list_store_set(model, &iter, j, s.c_str(), -1);
//             j++;
//         }
//     }
//     gtk_tree_view_set_model(parsing_table, GTK_TREE_MODEL(model));
//     gtk_tree_view_columns_autosize(parsing_table);

//     model = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

//     for (auto line : prod) {
//         GtkTreeIter iter;
//         gtk_list_store_insert(model, &iter, -1);
//         unsigned i = 0;
//         for (auto cell : line) {
//             gtk_list_store_set(model, &iter, i++, cell.c_str(), -1);
//         }
//     }

//     gtk_tree_view_set_model(enum_table, GTK_TREE_MODEL(model));

//     gtk_widget_show_all(pwindow);
// }

// void Interface::show_parsing_results(const std::array<std::string, 4>& results) const {
//     auto rbuilder = gtk_builder_new_from_file("analysis_result.ui");
//     auto rwindow = GTK_WIDGET(gtk_builder_get_object(rbuilder, "result_window"));

//     auto result = GTK_LABEL(gtk_builder_get_object(rbuilder, "result_status"));
//     auto details_view = GTK_TEXT_VIEW(gtk_builder_get_object(rbuilder, "details_view"));
//     auto details_buffer = gtk_text_view_get_buffer(details_view);
//     auto ps_view = GTK_TEXT_VIEW(gtk_builder_get_object(rbuilder, "ps_view"));
//     auto ps_buffer = gtk_text_view_get_buffer(ps_view);
//     auto ds_view = GTK_TEXT_VIEW(gtk_builder_get_object(rbuilder, "ds_view"));
//     auto ds_buffer = gtk_text_view_get_buffer(ds_view);

//     g_signal_connect(rwindow, "delete-event", G_CALLBACK(gtk_widget_destroy), (gpointer)rwindow);

//     gtk_label_set_text(result, results.at(0).c_str());

//     gtk_text_buffer_set_text(details_buffer, results.at(1).c_str(), -1);
    
//     gtk_text_buffer_set_text(ps_buffer, results.at(2).c_str(), -1);

//     gtk_text_buffer_set_text(ds_buffer, results.at(3).c_str(), -1);

//     gtk_widget_show_all(rwindow);
// }

// std::string Interface::open_file_dialog() {
//     return build_file_dialog(GTK_FILE_CHOOSER_ACTION_OPEN, "Open File", "Open");
// }

// std::string Interface::save_file_dialog() {
//     return build_file_dialog(GTK_FILE_CHOOSER_ACTION_SAVE, "Save File", "Save");
// }

// std::string Interface::build_file_dialog(const GtkFileChooserAction& action,
//                                          const std::string& title,
//                                          const std::string& ok_label) {
//     std::string filename = "";
//     auto file_chooser = gtk_file_chooser_dialog_new(title.c_str(), GTK_WINDOW(window),
//         action, "Cancel", GTK_RESPONSE_CANCEL, ok_label.c_str(), GTK_RESPONSE_ACCEPT, NULL);

//     if (action == GTK_FILE_CHOOSER_ACTION_SAVE) {
//         gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(file_chooser), true);
//     }
    
//     auto response = gtk_dialog_run(GTK_DIALOG(file_chooser));
//     if (response == GTK_RESPONSE_ACCEPT) {
//         char* file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));
//         filename = file;
//         g_free(file);
//     }
//     gtk_widget_destroy(file_chooser);
//     return filename;
// }