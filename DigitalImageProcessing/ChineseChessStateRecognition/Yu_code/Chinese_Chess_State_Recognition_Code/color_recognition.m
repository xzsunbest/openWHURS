function piece_infos = color_recognition(piece_infos)
    start_j = 1;
    end_j = 10;
    start_i = 1;
    end_i = 9;
    figure;
    rgb_red = [];
    rgb_black = [];
    rgb_query = [];
    loc_query = [];
    for j = start_j:end_j
        for i = start_i:end_i
            piece_info = piece_infos(i,j);
            if not (strcmp(piece_info.name, 'chariot')...
                    ||strcmp(piece_info.name, 'horse'))
                sum_piece = squeeze(sum(sum(piece_info.im(:,:,:))));
                sum_piece_norm = sum_piece/sum(sum_piece);
                if (strcmp(piece_info.color, 'black'))
                    scatter3(sum_piece_norm(1), sum_piece_norm(2), sum_piece_norm(3), 'b', '*')
                    rgb_black = [rgb_black sum_piece_norm];
                end
                if (strcmp(piece_info.color, 'red'))
                    scatter3(sum_piece_norm(1), sum_piece_norm(2), sum_piece_norm(3), 'r', '*')
                    rgb_red = [rgb_red sum_piece_norm];
                end
                hold on;
            else
                sum_piece = squeeze(sum(sum(piece_info.im(:,:,:))));
                sum_piece_norm = sum_piece/sum(sum_piece);
                scatter3(sum_piece_norm(1), sum_piece_norm(2), sum_piece_norm(3), 'g', '*')
                hold on;
                rgb_query = [rgb_query sum_piece_norm];
                loc_query = [loc_query [i;j]];

            end
        end
    end
    xlabel('r')
    ylabel('g')
    zlabel('b')

    [~, num_red] = size(rgb_red);
    [~, num_black] = size(rgb_black);
    if num_red == 0
        rgb_red = load('backup_rbg_red.mat');
        rgb_red = rgb_red.rgb_red;
    end
    if num_black == 0
        rgb_black = load('backup_rbg_black.mat');
        rgb_black = rgb_black.rgb_black;
    end
    [~, num_query] = size(rgb_query);
    for i = 1:num_query
        rgb_pt = rgb_query(:,i);
        if (find_min_distance(rgb_pt, rgb_black)<find_min_distance(rgb_pt, rgb_red))
            piece_infos(loc_query(1,i),loc_query(2,i)).color = 'black';
        else
            piece_infos(loc_query(1,i),loc_query(2,i)).color = 'red';
        end
    end
    figure;
    for j = start_j:end_j
        for i = start_i:end_i
            subplot(end_i,end_j,(i-1)*end_j+j);
            imshow(piece_infos(i,j).im);
            if (strcmp(piece_infos(i,j).color,'red'))
                put_label(30, 150, piece_infos(i,j).name, 'r');
            else
                put_label(30, 150, piece_infos(i,j).name, 'b');
            end
        end
    end
end

function min_dist = find_min_distance(pt, pt_set)
    [num_set, ~] = size(pt_set);
    min_dist = realmax;
    for i = 1:num_set
        dist = sqrt(sum((pt-pt_set(:,i)).^2));
        if dist < min_dist
            min_dist = dist;
        end
    end

end
